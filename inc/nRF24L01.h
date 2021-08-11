#pragma once
#include <GPIO.h>
#include <SPI.h>

#define NRF24_RX_BUFF_LEN 128

struct NRF24;

typedef void (*NRF24_handler)(struct NRF24 *m);

enum NRF24_Reg {
	NRF24_Reg_config = 0,
	NRF24_Reg_en_aa,
	NRF24_Reg_rxaddr,
	NRF24_Reg_aw,
	NRF24_Reg_retr,
	NRF24_Reg_rf_ch,
	NRF24_Reg_rf_setup,
	NRF24_Reg_status,
	NRF24_Reg_observe_tx,
	NRF24_Reg_rpd,
	NRF24_Reg_rx_addr_p0,
	NRF24_Reg_rx_addr_p1,
	NRF24_Reg_rx_addr_p2,
	NRF24_Reg_rx_addr_p3,
	NRF24_Reg_rx_addr_p4,
	NRF24_Reg_rx_addr_p5,
	NRF24_Reg_tx_addr,
	NRF24_Reg_rx_pw_p0,
	NRF24_Reg_rx_pw_p1,
	NRF24_Reg_rx_pw_p2,
	NRF24_Reg_rx_pw_p3,
	NRF24_Reg_rx_pw_p4,
	NRF24_Reg_rx_pw_p5,
	NRF24_Reg_fifo_status,
	NRF24_Reg_dynpd = 0x1c,
	NRF24_Reg_feature
};

// random check of registers' addresses
_Static_assert(NRF24_Reg_aw == 3, "Wrong address for register AW");
_Static_assert(NRF24_Reg_fifo_status == 0x17, "Wrong address for register FIFO_STATUS");
_Static_assert(NRF24_Reg_feature == 0x1d, "Wrong address for register FEATURE");

struct NRF24
{
	const struct SPI *spi;
	const struct GPIO_Pin *csn;
	const struct GPIO_Pin *ce;
	NRF24_handler on_rx;
	NRF24_handler on_tx_success;
	NRF24_handler on_tx_fail;
	Byte rxBuffer[NRF24_RX_BUFF_LEN];
	Byte bufferStart;
	Byte bufferLen;
	Byte status;
	Byte addrWidth;
};

bool NRF24_init(struct NRF24 *m);
void NRF24_pwr(struct NRF24 *m, bool state);
void NRF24_set_addr(struct NRF24 *m, const Byte *addr);
void NRF24_listen(struct NRF24 *m);
void NRF24_send(struct NRF24 *m, const Byte *addr, const Byte *payload, Byte len);
Byte NRF24_read(struct NRF24 *m, Byte *buff, Byte maxLen);
void NRF24_update(struct NRF24 *m);
Byte NRF24_status(struct NRF24 *m);
void NRF24_flush_rx(struct NRF24 *m);
void NRF24_flush_tx(struct NRF24 *m);
Byte NRF24_reg_get(struct NRF24 *m, Byte addr);
void NRF24_reg_get_long(struct NRF24 *m, Byte addr, Byte *val, Byte len);
void NRF24_reg_set(struct NRF24 *m, Byte addr, Byte val);
void NRF24_reg_set_long(struct NRF24 *m, Byte addr, const Byte *val, Byte len);
