.syntax unified
.thumb

.section .rodata
initVector:
	.word _STACK
	.word _reset_handler
	.word _NMI_handler
	.word _hardfault_handler
.org 0x00000054
	.word _RCC_handler
	.word _EXTI_handler
	.word _EXTI_handler
	.word _EXTI_handler
	.word _EXTI_handler
	.word _EXTI_handler
.org 0x0000009C
	.word _EXTI_handler
.org 0x000000E0
	.word _EXTI_handler
.org 0x00000118
	.word _TIM6_DAC_handler
	.word _TIM7_handler
.org 0x0000015C
	.word _USART6_handler
