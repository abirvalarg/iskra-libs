#pragma once
#include <type.h>
#include <RCC.h>

struct GPIO_Registers
{
	volatile Word MODER;
	volatile Word OTYPER;
	volatile Word OSPEEDR;
	volatile Word PUPDR;
	volatile Word IDR;
	volatile Word ODR;
	volatile Word BSRR;
	volatile Word LCKR;
	volatile Word AFRL;
	volatile Word AFRH;
};

struct GPIO
{
	struct GPIO_Registers *regs;
	volatile Word *busEnr;
	Byte enrPos;
};

struct GPIO_Pin
{
	const struct GPIO *hw;
	Byte pin;
};

extern struct GPIO_Registers GPIOA_BASE, GPIOB_BASE, GPIOC_BASE;
extern const struct GPIO GPIOA, GPIOB, GPIOC;

enum GPIO_PinMode {GPIO_PinMode_input = 0b00, GPIO_PinMode_output,
	GPIO_PinMode_alt, GPIO_PinMode_analog};
enum GPIO_PinOType {GPIO_PinOType_PullPush = 0, GPIO_PinOType_OpenDrain};
enum GPIO_PinPull {GPIO_PinPull_None = 0, GPIO_PinPull_Up, GPIO_PinPull_Down};

void GPIO_enable(const struct GPIO *base, bool state);
void GPIO_Pin_mode(const struct GPIO_Pin *pin, enum GPIO_PinMode mode);
void GPIO_Pin_otype(const struct GPIO_Pin *pin, enum GPIO_PinOType mode);
void GPIO_Pin_pull(const struct GPIO_Pin *pin, enum GPIO_PinPull mode);
void GPIO_Pin_write(const struct GPIO_Pin *pin, bool val);
bool GPIO_Pin_read(const struct GPIO_Pin *pin);
void GPIO_Pin_set_alt(const struct GPIO_Pin *pin, Byte alt);
void GPIO_Pin_speed(const struct GPIO_Pin *pin, Byte speed);
