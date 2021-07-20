#include <RCC.h>
#include <NVIC.h>
#include <misc.h>
#include <flash.h>

void set_PLL_values(unsigned int m, unsigned int n, unsigned int p, unsigned int q, Byte src)
{
	mask_irq();
	RCC_BASE->PLLCFGR =
		(m & 0b111111) |
		((n & ((1 << 10) - 1)) << 6) |
		((p & 0b11) << 16) |
		(q & 0b1111) << 24 |
		(src & 1) << 22;
	unmask_irq();
}

void set_bus_prescallers(unsigned int ahb, unsigned int apb1, unsigned int apb2)
{
	mask_irq();
	RCC_BASE->CFGR =
		((ahb & 0b1111) << 4) |
		((apb1 & 0b111) << 10) |
		((apb2 & 0b111) << 13);
	unmask_irq();
}

// cortex - 168MHz
// AHB - 168MHz
// APB1 - 42MHz
// APB1 timers - 84MHz
// APB2 - 84MHz
// APB2 timers - 168MHz
void set_max_freq()
{
	RCC_BASE->CIR |= BIT(11) | BIT(12);

	RCC_BASE->CR |= BIT(16); // enable HSE

	while(!(RCC_BASE->CIR & BIT(3))); // wait for HSE
	RCC_BASE->CIR |= BIT(19); // clear "ready" flag

	set_PLL_values(4, 168, 0, 8, 1); // PLL config
	RCC_BASE->CR |= BIT(24); // enable PLL

	while(!(RCC_BASE->CIR & BIT(4))); // wait for PLL
	RCC_BASE->CIR |= BIT(20); // clear "ready" flag

	FLASH.ACR = (FLASH.ACR & ~(0b111)) | 3; // set flash latency, not sure why
	set_bus_prescallers(0, 0b101, 0b100);
	RCC_BASE->CFGR |= 0b10; // set PLL as system clock

	while((RCC_BASE->CFGR & 0b1100) != 0b1000);

	RCC_BASE->CIR &= ~(BIT(11) | BIT(12));
}
