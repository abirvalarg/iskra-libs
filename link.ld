/* OUTPUT_FORMAT(binary) */
STARTUP(obj/libs/vector.s.o)

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1M,
	CCM (rw): ORIGIN = 0x10000000, LENGTH = 64K,
	SRAM1 (rwx): ORIGIN = 0x20000000, LENGTH = 112K,
	SRAM2 (rwx): ORIGIN = 0x2001c000, LENGTH = 16K
}

_STACK = ORIGIN(CCM) + LENGTH(CCM);

SECTIONS
{
	.text : ALIGN(4) {
		KEEP(obj/libs/vector.s.o(.rodata))
		*(.text)
		*(.rodata)
	} > FLASH

	_DATA_VAL_START = LOADADDR(.data);
	.data : ALIGN(4) {
		_DATA_START = .;
		*(.data)
		_DATA_END = .;
	} > SRAM1 AT>FLASH

	.bss : ALIGN(4) {
		_BSS_START = .;
		*(.bss)
		_BSS_END = .;
	} > SRAM1

	/DISCARD/ : ALIGN(4) {
		*(.data.buff)
	} > SRAM2
}
