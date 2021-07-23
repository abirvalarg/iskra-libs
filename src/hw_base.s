.syntax unified
.thumb

.macro BASE name, addr
	.equ \name, \addr
	.global \name
.endm

.equ GPIOA_BASE, 0x40020000
.equ GPIOB_BASE, 0x40020400
.equ GPIOC_BASE, 0x40020800
.equ TIM6_BASE, 0x40001000
.equ TIM7_BASE, 0x40001400
.equ USART6_BASE, 0x40011400
BASE SPI1_BASE, 0x40013000
BASE SPI2_BASE, 0x40003800

.equ NVIC_ISER, 0xE000E100
.equ FLASH, 0x40023C00
BASE EXTI_BASE, 0x40013C00
BASE SYSCFG_BASE, 0x40013800

.global GPIOA_BASE
.global GPIOB_BASE
.global GPIOC_BASE
.global TIM6_BASE
.global TIM7_BASE
.global USART6_BASE

.global NVIC_ISER
.global FLASH
