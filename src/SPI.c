#include <SPI.h>
#include <RCC.h>

const struct SPI SPI1 = {
	.hw = &SPI1_BASE,
	.busEnr = &RCC_BASE->APB2ENR,
	.enrPos = 12
};
