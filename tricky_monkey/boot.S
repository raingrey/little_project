#set magic number to 0x1badb002 to identified by bootloader
.set MAGIC, 0x1BADB002
#MAGIC字段是给bootloader识别的，是加载内核的起始点

#set flags to 0
.set FLAGS, 0
#FLAG字段置0表示随内核启动的模块都是4KB页对齐

#set the check sum 
.set CHECKSUM, -(MAGIC+FLAGS)
#CHECKSUM字段就是MAGIC和FLAGS的和

#set multiboot enabled
.section .multiboot

#define type to long for each data defined as above
.long MAGIC
.long FLAGS
.long CHECKSUM

#set the stack bottom
stackBottom:

#define the maximum size of stack to 512bytes
.skip 512

#set the stack top which grows from higher to lower
stackTop:

.section .text
.global _start
.type _start,@function


_start:
  #assign current stack pointer location to stackTop
  mov $stackTop,%esp

  #call the kernel main source
  call KERNEL_MAIN

  cli

#put system in infinite loop
hltLoop:
  hlt
  jmp hltLoop

#这意思应该是代码的size
.size _start, . - _start

#上面创建了512byte的站，从stackBottom到stackTop，在_start保存了当前的栈指针，然后调到了内核主函数。
