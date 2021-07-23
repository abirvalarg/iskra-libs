#include "USART.h"
#include "RCC.h"

const struct USART USART6 = {
	.hw = &USART6_BASE,
	.busEnr = &RCC_BASE->APB2ENR,
	.enrPos = 5
};

void USART_enable(const struct USART *usart, bool state)
{
	if(state)
	{
		*usart->busEnr |= 1 << usart->enrPos;
		usart->hw->CR1 |= 1 << 13;
	}
	else
		*usart->busEnr &= ~(1 << usart->enrPos);
}

void USART_config(const struct USART *usart, struct USART_Config cfg)
{
	usart->hw->CR1 =
		cfg.tx << 3 |
		cfg.rx << 2 |
		cfg.txInterrupt << 7 |
		cfg.rxInterrupt << 5 |
		(1 << 13);
	usart->hw->BRR = cfg.baudRate;
}

void USART_send_str(const struct USART *usart, const char *data)
{
	char ch;
	Word pos = 0;
	while((ch = data[pos++]))
	{
		while(!(usart->hw->SR & (1 << 7)));
		usart->hw->DR = ch;
	}
}

void USART_send_word(const struct USART *usart, Word data)
{
	char buff[11];
	buff[10] = 0;
	buff[9] = '0';
	int pos = 9;
	while(data)
	{
		buff[pos--] = (data % 10) + '0';
		data /= 10;
		if(pos == -1)
			break;
	}
	USART_send_str(usart, buff + pos + (pos != 9));
}

void USART_send_byte_bin(const struct USART *usart, Byte data)
{
	char buff[9];
	buff[8] = 0;
	for(int i = 7; i >= 0; i--)
	{
		buff[i] = (data & 1) + '0';
		data >>= 1;
	}
	USART_send_str(usart, buff);
}

void USART_send_byte_hex(const struct USART *usart, Byte data)
{
	char buff[3];
	buff[2] = 0;
	for(int i = 0; i < 2; i++)
	{
		Byte tmp = data & 0xf;
		tmp += (tmp > 9) ? 'A' - 10 : '0';
		buff[1 - i] = tmp;
		data >>= 4;
	}
	USART_send_str(usart, buff);
}
