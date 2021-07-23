#pragma once
#include <type.h>

struct SPI_Registers
{
	volatile Word CR1;
	volatile Word CR2;
	volatile Word SR;
	volatile Word DR;
	volatile Word CRCPR;
	volatile Word RXCRCR;
	volatile Word TXCRCR;
	volatile Word I2SCFGR;
	volatile Word I2SPR;
};

struct SPI
{
	struct SPI_Registers *hw;
	volatile Word *busEnr;
	Byte enrPos;
};

extern struct SPI_Registers SPI1_BASE, SPI2_BASE;

extern const struct SPI SPI1, SPI2;

void SPI_enable(const struct SPI *spi, bool state);
HalfWord SPI_exchange(const struct SPI *spi, HalfWord tx);
