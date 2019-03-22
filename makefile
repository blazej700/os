
#export PATH="$HOME/opt/cross/bin:$PATH" exprot cross compiler prefix before make

os-image.bin: boot.bin kernel.bin
	cat $^ > os-image.bin
boot.bin: boot.o
	i686-elf-ld -Ttext 0x7c00 --oformat binary -o boot.bin boot.o
boot.o: boot.s
	i686-elf-as -o boot.o boot.s
kernel.bin:kernel_entry.o kernel.o
	i686-elf-ld -o kernel.bin -Ttext 0x1000 $^ --oformat binary  #i686-elf-ld

kernel.o: kernel.c 
	i686-elf-gcc -ffreestanding -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra
kernel_entry.o: kernel_entry.s
	i686-elf-as -o kernel_entry.o kernel_entry.s

