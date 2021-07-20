#pragma once
#include <type.h>

struct RCC
{
	volatile Word CR;
	volatile Word PLLCFGR;
	volatile Word CFGR;
	volatile Word CIR;
	volatile Word AHB1RSTR;
	volatile Word AHB2RSTR;
	volatile Word AHB3RSTR;
	volatile Word _reserved0;
	volatile Word APB1RSTR;
	volatile Word APB2RSTR;
	volatile Word _reserved1;
	volatile Word _reserved2;
	volatile Word AHB1ENR;
	volatile Word AHB2ENR;
	volatile Word AHB3ENR;
	volatile Word _reserved3;
	volatile Word APB1ENR;
	volatile Word APB2ENR;
	volatile Word _reserved4;
	volatile Word _reserved5;
	volatile Word AHB1LPENR;
	volatile Word AHB2LPENR;
	volatile Word AHB3LPENR;
	volatile Word _reserved6;
	volatile Word APB1LPENR;
	volatile Word APB2LPENR;
	volatile Word _reserved7;
	volatile Word _reserved8;
	volatile Word BDCR;
	volatile Word CSR;
	volatile Word _reserved9;
	volatile Word _reserved10;
	volatile Word SSCGR;
	volatile Word PLLI2SCFGR;
};

#define RCC_BASE ((struct RCC*)0x40023800)

void set_PLL_values(unsigned m, unsigned n, unsigned p, unsigned q, Byte src);
void set_bus_prescallers(unsigned ahb, unsigned apb1, unsigned apb2);
void set_max_freq();
