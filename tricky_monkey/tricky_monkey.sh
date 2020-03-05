#!/bin/bash
#Requirments
#assemble boot.s
as --32 boot.S -o boot.o
#Assembler-GNU Assmbler(gas)命令是as，还有一个Netwide Assmbler命令是nasm，都是.s转.o

#compile kernel.c
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#-ffreestanding 可以没有标准库，且对main()函数没有要求，用于操作系统内核。

#linking the kernel with kernel.o and boot.o
gcc -m32 -T linker.ld -o MyOS.bin -ffreestanding -O2 -nostdlib kernel.o boot.o -lgcc 
#-lgcc代表连接器将连接GCC标准支持库libgcc.a，-lm是libm.a(数学库),-lc是libc.a

#check MyOS.bin is x86 multiboot file or not
grub-file --is-x86-multiboot MyOS.bin
#这是grub的工具,还grub2-mkrescue也是

#building the iso file
mkdir -p isodir/boot/grub
cp MyOS.bin isodir/boot/MyOS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o MyOS.iso isodir

#run it in qemu
qemu-system-x86_64 -cdrom MyOS.iso


