.syntax unified
.thumb

.section .text

.global mask_irq
.thumb_func
mask_irq:
	mov r0, 0x1
	msr primask, r0
	bx lr

.global unmask_irq
.thumb_func
unmask_irq:
	mov r0, 0x0
	msr primask, r0
	bx lr
