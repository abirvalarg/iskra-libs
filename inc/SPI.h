#pragma once
#include <type.h>

struct SPI_Registers
{
};

struct SPI
{
	struct SPI_Registers *hw;
	volatile Word *busEnr;
	Byte enrPos;
};

extern struct SPI_Registers SPI1_BASE;

extern const struct SPI SPI1;
