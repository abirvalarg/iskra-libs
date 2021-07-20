#pragma once
#include <type.h>

struct SYSCFG
{
	volatile Word MEMRMP;
	volatile Word PMC;
	volatile Word EXTICR[4];
	volatile Word CMPCR;
};

extern struct SYSCFG SYSCFG_BASE;

void SYSCFG_enable(bool state);
