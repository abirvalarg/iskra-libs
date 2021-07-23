#include <SYSCFG.h>
#include <RCC.h>

void SYSCFG_enable(bool state)
{
	if(state)
		RCC_BASE->APB2ENR |= BIT(14);
	else
		RCC_BASE->APB2ENR &= ~BIT(14);
}

void SYSCFG_comp_cell(bool state)
{
	if(state)
	{
		SYSCFG_BASE.CMPCR = 1;
		while(!(SYSCFG_BASE.CMPCR & BIT(8)));
	}
	else
		SYSCFG_BASE.CMPCR = 0;
}
