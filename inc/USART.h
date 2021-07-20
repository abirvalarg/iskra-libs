#pragma once
#include "type.h"

struct USART_Registers
{
	volatile Word SR;
	volatile Word DR;
	volatile Word BRR;
	volatile Word CR1;
	volatile Word CR2;
	volatile Word CR3;
	volatile Word GTPR;
};

struct USART
{
	struct USART_Registers *hw;
	volatile Word *busEnr;
	Byte enrPos;
};

struct USART_Config
{
	HalfWord baudRate;
	bool tx : 1;
	bool rx : 1;
	bool txInterrupt : 1;
	bool rxInterrupt : 1;
};

extern struct USART_Registers USART6_BASE;

extern const struct USART USART6;

void USART_enable(const struct USART *usart, bool state);
void USART_config(const struct USART *usart, struct USART_Config cfg);
void USART_send_str(const struct USART *usart, const char *data);
void USART_send_word(const struct USART *usart, Word data);
