#include <SYSCFG.h>
#include <RCC.h>

void SYSCFG_enable(bool state)
{
	if(state)
		RCC_BASE->APB2ENR |= BIT(14);
	else
		RCC_BASE->APB2ENR &= ~BIT(14);
}
