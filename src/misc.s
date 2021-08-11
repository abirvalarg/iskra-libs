.syntax unified
.thumb

.section .bss
__irqLockLevel:
	.byte 0

.section .text

.global mask_irq
.thumb_func
mask_irq:
	mov r0, 0x1
	msr primask, r0
	ldr r0, =__irqLockLevel
	ldrb r1, [r0]
	add r1, 0x1
	strb r1, [r0]
	bx lr

.global unmask_irq
.thumb_func
unmask_irq:
	ldr r0, =__irqLockLevel
	ldrb r1, [r0]
	subs r1, 0x1
	// r1 <= 1 -> unlock, r1 = 0
	bhi 0f
	mov r1, 0x0
0:

	strb r1, [r0]

	bhi 0f
	msr primask, r1
0:

	bx lr
