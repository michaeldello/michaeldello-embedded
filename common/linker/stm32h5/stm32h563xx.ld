/*
ENTRY(Reset_handler)
*/

/* Flash 2M, SRAM 640K */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 640K
}

/* Top of stack, end of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Symbols for run-time init */
_sidata = LOADADDR(.data);

/* Filled by linker */
_edata = 0;
_ebss  = 0;

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
    KEEP(*(.init))
    KEEP(*(.fini))
  } > FLASH

  .ARM.extab : { *(.ARM.extab*) } > FLASH
  .ARM.exidx : { *(.ARM.exidx*) } > FLASH

  .data : AT (LOADADDR(.text) + SIZEOF(.text) + SIZEOF(.ARM.extab) + SIZEOF(.ARM.exidx))
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  ._user_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE(end = .);

  } > RAM
}