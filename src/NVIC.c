#include <NVIC.h>

void NVIC_unmask(int pos)
{
	int block = pos / 32;
	NVIC_ISER[block] = 1 << (pos % 32);
}
