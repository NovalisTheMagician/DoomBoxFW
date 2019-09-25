MEMORY 
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 192K

    /*SDRAM is 32MB*/
    SDRAM (rwx) : ORIGIN = 0xD0000000, LENGTH = 8M

    /*SPIFLASH is a dual bank 16MB (32MB in total)*/
    /*SPIFLASH (rx) : ORIGIN = 0x90000000, LENGTH = 32M*/
}

SECTIONS
{
    .text : 
    {
        KEEP (*(.vectors))
        *(.text .text.*)
        /**(.text .text.* .gnu.linkonce.t.*)*/
		*(.glue_7t) *(.glue_7)
        *(.rodata .rodata*)
		/**(.rodata .rodata* .gnu.linkonce.r.*)*/
    } >FLASH

    . = ALIGN(4);
    _etext = .;

    .data :
    {
        _sdata = .;
        *(.data .data.*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT >FLASH
    _sflashdata = LOADADDR(.data);

    .bss :
    {
        _sbss = .;
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = .;
    } >RAM

    .sdram :
    {
        _ssdram = .;
        *(.sdram .sdram.*)
    } >SDRAM
    _esdram = ORIGIN(SDRAM) + LENGTH(SDRAM);

    _estack = ORIGIN(RAM) + LENGTH(RAM);
}