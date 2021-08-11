#include "GPIO.h"
#include "SPI.h"
#include <nRF24L01.h>
#include <misc.h>

bool NRF24_init(struct NRF24 *m)
{
	if(m->addrWidth < 3 || m->addrWidth > 5)
		return false;

	GPIO_Pin_mode(m->ce, GPIO_PinMode_output);
	GPIO_Pin_mode(m->csn, GPIO_PinMode_output);

	GPIO_Pin_write(m->ce, 0);
	GPIO_Pin_write(m->csn, 1);

	for(int i = 0; i < 100; i++)
		asm("nop");

	NRF24_pwr(m, 0);
	NRF24_flush_rx(m);
	NRF24_flush_tx(m);
	NRF24_reg_set(m, NRF24_Reg_status, m->status & 0b01110000);

	NRF24_reg_set(m, NRF24_Reg_aw, m->addrWidth - 2);
	NRF24_reg_set(m, NRF24_Reg_feature, BIT(2));

	return true;
}

void NRF24_pwr(struct NRF24 *m, bool state)
{
	Byte cr = NRF24_reg_get(m, NRF24_Reg_config);
	if(state)
		cr |= BIT(1);
	else
		cr &= ~BIT(1);
	NRF24_reg_set(m, NRF24_Reg_config, cr);
}

void NRF24_set_addr(struct NRF24 *m, const Byte *addr)
{
	NRF24_reg_set_long(m, NRF24_Reg_rx_addr_p1, addr, m->addrWidth);
}

void NRF24_listen(struct NRF24 *m)
{
	Byte cr = NRF24_reg_get(m, NRF24_Reg_config);
	cr |= BIT(0);
	NRF24_reg_set(m, NRF24_Reg_config, cr);
	NRF24_reg_set(m, NRF24_Reg_dynpd, BIT(1));
	GPIO_Pin_write(m->ce, 1);
}

void NRF24_send(struct NRF24 *m, const Byte *addr, const Byte *payload, Byte len)
{
	GPIO_Pin_write(m->ce, 0);
	Byte cr = NRF24_reg_get(m, NRF24_Reg_config);
	cr &= ~BIT(0);
	NRF24_reg_set(m, NRF24_Reg_config, cr);

	Byte dynpd = NRF24_reg_get(m, NRF24_Reg_dynpd);
	dynpd |= BIT(0);
	NRF24_reg_set(m, NRF24_Reg_dynpd, dynpd);

	NRF24_reg_set_long(m, NRF24_Reg_rx_addr_p0, addr, m->addrWidth);
	NRF24_reg_set_long(m, NRF24_Reg_tx_addr, addr, m->addrWidth);

	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, 0b10100000);
	for(int i = 0; i < len; i++)
		SPI_exchange(m->spi, payload[i]);
	GPIO_Pin_write(m->csn, 1);
	GPIO_Pin_write(m->ce, 1);
}

Byte NRF24_read(struct NRF24 *m, Byte *buff, Byte maxLen)
{
	Byte len = m->bufferLen;
	if(maxLen < len)
		len = maxLen;
	for(Byte writePos = 0; writePos < len; writePos++)
	{
		buff[writePos] = m->rxBuffer[m->bufferStart++];
		m->bufferStart %= NRF24_RX_BUFF_LEN;
	}
	m->bufferLen -= len;
	return len;
}

void NRF24_update(struct NRF24 *m)
{
	mask_irq();
	Byte st = NRF24_status(m);
	if(st & BIT(6))
	{
		Byte freeSpace = NRF24_RX_BUFF_LEN - m->bufferLen;
		GPIO_Pin_write(m->csn, 0);
		SPI_exchange(m->spi, 0b01100000);
		Byte rxLen = SPI_exchange(m->spi, 0);
		GPIO_Pin_write(m->csn, 1);
		if(rxLen > freeSpace)
			rxLen = freeSpace;
		GPIO_Pin_write(m->csn, 0);
		SPI_exchange(m->spi, 0b01100001);

		Byte writePos = (m->bufferStart + m->bufferLen) % NRF24_RX_BUFF_LEN;
		for(int i = 0; i < rxLen; i++)
		{
			m->rxBuffer[writePos++] = SPI_exchange(m->spi, 0);
			writePos %= NRF24_RX_BUFF_LEN;
		}
		m->bufferLen += rxLen;

		GPIO_Pin_write(m->csn, 1);
		if(m->on_rx)
			m->on_rx(m);
	}
	if((st & BIT(5)) && m->on_tx_success)
		m->on_tx_success(m);
	if((st & BIT(4)) && m->on_tx_fail)
		m->on_tx_fail(m);
	NRF24_reg_set(m, NRF24_Reg_status, st);
	unmask_irq();
}

Byte NRF24_status(struct NRF24 *m)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, 0xff);
	GPIO_Pin_write(m->csn, 1);
	return m->status;
}

void NRF24_flush_rx(struct NRF24 *m)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, 0b11100010);
	GPIO_Pin_write(m->csn, 1);
}

void NRF24_flush_tx(struct NRF24 *m)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, 0b11100001);
	GPIO_Pin_write(m->csn, 1);
}

Byte NRF24_reg_get(struct NRF24 *m, Byte addr)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, addr & 0b11111);
	Byte val = SPI_exchange(m->spi, 0);
	GPIO_Pin_write(m->csn, 1);
	return val;
}

void NRF24_reg_set(struct NRF24 *m, Byte addr, Byte val)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, BIT(5) | (addr & 0b11111));
	SPI_exchange(m->spi, val);
	GPIO_Pin_write(m->csn, 1);
}

void NRF24_reg_set_long(struct NRF24 *m, Byte addr, const Byte *val, Byte len)
{
	GPIO_Pin_write(m->csn, 0);
	m->status = SPI_exchange(m->spi, BIT(5) | (addr & 0b11111));
	for(int i = 0; i < len; i++)
		SPI_exchange(m->spi, val[len - i]);
	GPIO_Pin_write(m->csn, 1);
}
