#pragma once
#include <type.h>

extern Word NVIC_ISER[8];
extern Word NVIC_ICER[8];

void NVIC_unmask(int pos);
void NVIC_mask(int pos);
