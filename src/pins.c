#include "pins.h"
#include "GPIO.h"

const struct GPIO_Pin LED1 = {
	.hw = &GPIOB,
	.pin = 6
};

const struct GPIO_Pin LED2 = {
	.hw = &GPIOB,
	.pin = 7
};

const struct GPIO_Pin BTN1 = {
	.hw = &GPIOC,
	.pin = 4
};

const struct GPIO_Pin P0 = {
	.hw = &GPIOB,
	.pin = 11
};

const struct GPIO_Pin P1 = {
	.hw = &GPIOB,
	.pin = 10
};

const struct GPIO_Pin P2 = {
	.hw = &GPIOA,
	.pin = 6
};

const struct GPIO_Pin P3 = {
	.hw = &GPIOA,
	.pin = 7
};

const struct GPIO_Pin P4 = {
	.hw = &GPIOC,
	.pin = 3
};

const struct GPIO_Pin P5 = {
	.hw = &GPIOB,
	.pin = 1
};

const struct GPIO_Pin P6 = {
	.hw = &GPIOB,
	.pin = 0
};

const struct GPIO_Pin P7 = {
	.hw = &GPIOC,
	.pin = 2
};

const struct GPIO_Pin P8 = {
	.hw = &GPIOC,
	.pin = 6
};

const struct GPIO_Pin P9 = {
	.hw = &GPIOC,
	.pin = 7
};

const struct GPIO_Pin A0 = {
	.hw = &GPIOA,
	.pin = 0
};

const struct GPIO_Pin A1 = {
	.hw = &GPIOA,
	.pin = 1
};

const struct GPIO_Pin A4 = {
	.hw = &GPIOA,
	.pin = 4
};

const struct GPIO_Pin A5 = {
	.hw = &GPIOA,
	.pin = 5
};
