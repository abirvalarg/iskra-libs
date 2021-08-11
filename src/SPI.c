#include <SPI.h>
#include <RCC.h>

const struct SPI SPI1 = {
	.hw = &SPI1_BASE,
	.busEnr = &RCC_BASE->APB2ENR,
	.enrPos = 12
};

const struct SPI SPI2 = {
	.hw = &SPI2_BASE,
	.busEnr = &RCC_BASE->APB2ENR,
	.enrPos = 14
};

void SPI_enable(const struct SPI *spi, bool state)
{
	if(state)
		*spi->busEnr |= 1 << spi->enrPos;
	else
		*spi->busEnr &= ~(1 << spi->enrPos);
}

HalfWord SPI_exchange(const struct SPI *spi, HalfWord tx)
{
	while(!(spi->hw->SR & BIT(1)));
	spi->hw->DR = tx;
	while(!(spi->hw->SR & BIT(0)));
	for(int i = 0; i < 150; i++)
		asm("nop");
	return spi->hw->DR;
}
