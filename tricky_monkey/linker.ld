/*entry point of our kernel*/
ENTRY(_start)

/*我认为，在ld文件中，SECTIONS的定义应该是代码中各个段的空间*/
/*如.text段占4K,4K对齐，其中包含.multiboot和.text两段*/
SECTIONS
{
  /*we need 1MB of space at least*/
  . = 1M;
  
  /*text section*/
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  /*read only data section*/
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /*data section*/
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /*bss section*/
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

}
