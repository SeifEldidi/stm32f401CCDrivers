/* Entry Point Jump to reset handler then init MCU then to main()*/
ENTRY(Reset_Handler)
/*Set the first address of the stack memory*/
_estack = ORIGIN(SRAM) + LENGTH(SRAM) ;

_MIN_HEAP_SIZE  = 0x200;
_MIN_STACK_SIZE = 0x400;

/*define Memory in LinkerScript for object relocation */
MEMORY
{
    SRAM   (xrw)  : ORIGIN = 0x20000000, LENGTH = 64K
    FLASH  (rx)   : ORIGIN = 0x08000000, LENGTH = 256K
}

/*Build Different Memory Sections*/
SECTIONS
{
    /*First Entry in Linker Script IVT section*/
    .isr_vector :
    {
        . = ALIGN(4);
        *( isr_vector)
        . = ALIGN(4);
    } >FLASH
    /*Text section in FlashMemory */
    .text :
    {
        . = ALIGN(4);
        *(.text);
        *(.text*);
        . = ALIGN(4);
    } >FLASH
    /*Ro-data Section in Flash Memory*/

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata);
        *(.rodata*);
        . = ALIGN(4);
    } >FLASH
    
    /* will be used by startupcode to init the .data section */
    _sidata = LOADADDR(.data);
    /*.data section in RAM which is a copy by startup code to RAM */
    .data :
    {
        _sdata = .;
        . = ALIGN(4);
        *(.data);
        *(.data*);
        . = ALIGN(4);
        _edata = .;
    } >SRAM AT >FLASH
    /* bss section in read only memory */
    .bss :
    {
        _sbss = .;
        . = ALIGN(4);
        *(.bss);
        *(.bss*);
        . = ALIGN(4);
        _ebss = .;
    } >SRAM
    /* Stack_Heap Section in Memory */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE( end = . );
        PROVIDE( _end = . );
        . = . + _MIN_HEAP_SIZE;
        . = . + _MIN_STACK_SIZE;
        . = ALIGN(8);
    } >SRAM
}
