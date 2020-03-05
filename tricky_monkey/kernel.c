#include "kernel.h"

static unsigned short VGA_DefaultEntry(unsigned char to_print){
  return (unsigned short)to_print|(unsigned short)WHITE_COLOR<<8;
}
void KERNEL_MAIN()
{
  TERMINAL_BUFFER = (unsigned short*) VGA_ADDRESS;
  TERMINAL_BUFFER[0] = VGA_DefaultEntry('H');
  TERMINAL_BUFFER[1] = VGA_DefaultEntry('e');
  TERMINAL_BUFFER[2] = VGA_DefaultEntry('l');
  TERMINAL_BUFFER[3] = VGA_DefaultEntry('l');
  TERMINAL_BUFFER[4] = VGA_DefaultEntry('o');
  TERMINAL_BUFFER[5] = VGA_DefaultEntry(' ');
  TERMINAL_BUFFER[6] = VGA_DefaultEntry('W');
  TERMINAL_BUFFER[7] = VGA_DefaultEntry('o');
  TERMINAL_BUFFER[8] = VGA_DefaultEntry('r');
  TERMINAL_BUFFER[9] = VGA_DefaultEntry('l');
  TERMINAL_BUFFER[10] = VGA_DefaultEntry('d');
}
