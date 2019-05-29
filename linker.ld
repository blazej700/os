OUTPUT_FORMAT("binary")
ENTRY(kstart)
SECTIONS
{
  . = 0x01000;

  .text :
  {
    *(.text*)
    *(.rodata)
  }

  .data  :
  {
    start_ctors = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    end_ctors = .;

    *(.data)
  }

  .bss  :
  {
    *(.bss)
  }

  /DISCARD/ : { *(.fini_array*) *(.comment) }
}