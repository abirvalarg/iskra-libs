#pragma once
#include <type.h>

struct EXTI
{
	volatile Word IMR;
	volatile Word EMR;
	volatile Word RTSR;
	volatile Word FTSR;
	volatile Word SWIER;
	volatile Word PR;
};

extern struct EXTI EXTI_BASE;

enum EXTI_Event {EXTI_Event_none=0, EXTI_Event_fall, EXTI_Event_rise, EXTI_Event_both};

typedef void (*EXTI_Handler)(Byte gpioId, Byte pin);
EXTI_Handler EXTI_on_event(EXTI_Handler, enum EXTI_Event event, Byte gpioId, Byte pin);
