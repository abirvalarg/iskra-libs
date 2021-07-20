#pragma once
#include <type.h>

struct FLASH
{
	volatile Word ACR;
	volatile Word KEYR;
	volatile Word OPTKEYR;
	volatile Word SR;
	volatile Word CR;
	volatile Word OPTCR;
};

extern struct FLASH FLASH;
