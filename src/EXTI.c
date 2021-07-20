#include <EXTI.h>
#include <SYSCFG.h>
#include <misc.h>

EXTI_Handler _EXTI_handlers[16];
Byte _EXTI_map[16];

EXTI_Handler EXTI_on_event(EXTI_Handler handler, enum EXTI_Event event, Byte gpioId, Byte pin)
{
	mask_irq();

	EXTI_Handler old = _EXTI_handlers[pin];
	_EXTI_handlers[pin] = handler;

	if(handler)
	{
		EXTI_BASE.IMR |= BIT(pin);
		if(event & EXTI_Event_rise)
			EXTI_BASE.RTSR |= BIT(pin);
		else
			EXTI_BASE.RTSR &= ~BIT(pin);
		if(event & EXTI_Event_fall)
			EXTI_BASE.FTSR |= BIT(pin);
		else
			EXTI_BASE.FTSR &= ~BIT(pin);

		Byte reg = (pin >> 2) & 0b11;
		Byte pos = (pin & 0b11) * 4;
		Word tmp = SYSCFG_BASE.EXTICR[reg];
		tmp &= ~(0b1111 << pos);
		tmp |= (gpioId & 0b1111) << pos;
		SYSCFG_BASE.EXTICR[reg] = tmp;

		_EXTI_map[pin] = gpioId & 0b1111;
	}
	else
	{
		EXTI_BASE.IMR &= ~BIT(pin);
	}

	unmask_irq();
	return old;
}
