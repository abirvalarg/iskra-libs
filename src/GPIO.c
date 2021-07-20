#include "GPIO.h"
#include "RCC.h"
#include "misc.h"

const struct GPIO GPIOA = {
	.regs = &GPIOA_BASE,
	.busEnr = &RCC_BASE->AHB1ENR,
	.enrPos = 0
};

const struct GPIO GPIOB = {
	.regs = &GPIOB_BASE,
	.busEnr = &RCC_BASE->AHB1ENR,
	.enrPos = 1
};

const struct GPIO GPIOC = {
	.regs = &GPIOC_BASE,
	.busEnr = &RCC_BASE->AHB1ENR,
	.enrPos = 2
};

void GPIO_enable(const struct GPIO *hw, bool state)
{
	mask_irq();
	if(state)
		*hw->busEnr |= (1 << hw->enrPos);
	else
		*hw->busEnr &= ~(1 << hw->enrPos);
	unmask_irq();
}

void GPIO_Pin_mode(const struct GPIO_Pin *pin, enum GPIO_PinMode mode)
{
	mask_irq();
	Word tmp = pin->hw->regs->MODER;
	tmp &= ~(0b11 << (pin->pin * 2));
	tmp |= mode << (pin->pin * 2);
	pin->hw->regs->MODER = tmp;
	unmask_irq();
}

void GPIO_Pin_otype(const struct GPIO_Pin *pin, enum GPIO_PinOType mode)
{
	volatile Word *reg = &pin->hw->regs->OTYPER;
	mask_irq();
	if(mode)
		*reg |= 1 << pin->pin;
	else
		*reg &= ~(1 << pin->pin);
	unmask_irq();
}

void GPIO_Pin_pull(const struct GPIO_Pin *pin, enum GPIO_PinPull mode)
{
	mask_irq();
	Word tmp = pin->hw->regs->PUPDR;
	tmp &= ~(0b11 << (pin->pin * 2));
	tmp |= mode << (pin->pin * 2);
	pin->hw->regs->PUPDR = tmp;
	unmask_irq();
}

void GPIO_Pin_write(const struct GPIO_Pin *pin, bool val)
{
	mask_irq();
	volatile Word *reg = &pin->hw->regs->ODR;
	if(val)
		*reg |= 1 << pin->pin;
	else
		*reg &= ~(1 << pin->pin);
	unmask_irq();
}

bool GPIO_Pin_read(const struct GPIO_Pin *pin)
{
	return (pin->hw->regs->IDR >> pin->pin) & 1;
}

void GPIO_Pin_set_alt(const struct GPIO_Pin *pin, Byte alt)
{
	volatile Word *reg = (pin->pin & 0b1000) ? &pin->hw->regs->AFRH : &pin->hw->regs->AFRL;
	Word tmp = *reg & ~(0b1111 << ((pin->pin & 0b111) * 4));
	tmp |= (alt & 0b1111) << ((pin->pin & 0b111) * 4);
	*reg = tmp;
}
