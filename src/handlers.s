.syntax unified
.thumb

.macro	HANDLER name
	.equ \name, _default_handler
	.weak \name
.endm

HANDLER _reset_handler
HANDLER _NMI_handler
HANDLER _hardfault_handler
HANDLER _TIM6_DAC_handler
HANDLER _TIM7_handler
HANDLER _RCC_handler
HANDLER _USART6_handler
HANDLER _EXTI_handler

.section .text

.global _reset_handler
.thumb_func
.align 2
_reset_handler:
	// initialize .data
	ldr r0, =_DATA_START
	ldr r1, =_DATA_VAL_START
	ldr r2, =_DATA_END

0:
	cmp r0, r2
	beq 1f
	ldrb r3, [r1], 1
	strb r3, [r0], 1
	b 0b
1:

	// initialize .bss
	ldr r0, =_BSS_START
	ldr r1, =_BSS_END
	ldrb r3, =0x00

0:
	cmp r0, r1
	beq 1f
	strb r3, [r0], 1
	b 0b
1:

	bl start
0:
	wfi
	b.n 0b

.global _default_handler
.weak _default_handler
.thumb_func
.align 2, 0x46c0
_default_handler:
	b.n _default_handler
	nop

.global _TIM6_DAC_handler
.thumb_func
_TIM6_DAC_handler:
	push {lr}

	/*
	ldr r0, =LED2
	ldr r1, =1
	bl GPIO_Pin_write
	*/

	ldr r0, =TIM6
	bl _timer_handler

	pop {lr}
	bx lr

.global _TIM7_handler
.thumb_func
_TIM7_handler:
	push {lr}

	/*
	ldr r0, =LED2
	ldr r1, =0
	bl GPIO_Pin_write
	*/

	ldr r0, =TIM7
	bl _timer_handler

	pop {lr}
	bx lr

.global _timer_handler
.thumb_func
.align 2
_timer_handler:
	push {r0, r1, r4, lr}

	mov r4, r0
	bl Timer_reset_int
	cmp r0, 0
	beq 0f
	
	ldr r1, [r4, 3 * 4]
	ldr r1, [r1]
	cmp r1, 0
	beq 0f
	mov r0, r4
	blx r1

0:
	pop {r0, r1, r4, lr}
	bx lr
	nop.n

.global _EXTI_handler
.thumb_func
_EXTI_handler:
	push {r4, r5, r6, lr}

	ldr r4, =EXTI_BASE
	ldr r5, [r4, 0x14]
	ldr r6, =0x0

0:
	cmp r6, 0x10
	beq 9f
	tst r5, 0x1
	beq 1f

	ldr r1, =_EXTI_map
	ldr r0, [r1, r6]
	mov r1, r6
	ldr r3, =_EXTI_handlers
	ldr r3, [r3, r6, lsl 0x2]
	blx r3
	ldr r0, =0x1
	lsl r0, r0, r6
	str r0, [r4, 0x14]

1:
	lsr r5, r5, 0x1
	add r6, 1
	b 0b
9:

	pop {r4, r5, r6, lr}
	bx lr
