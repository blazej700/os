
#export PATH="$HOME/opt/cross/bin:$PATH" #exprot cross compiler prefix before make

os-image.bin: boot.bin kernel.bin
	cat $^ > os-image.bin
boot.bin: boot.o
	i686-elf-ld -Ttext 0x7c00 --oformat binary -o boot.bin boot.o
boot.o: boot.s
	i686-elf-as -o boot.o boot.s
kernel.bin:kernel_entry.o kernel.o ports.o util.o screen.o
	i686-elf-ld -o kernel.bin -Ttext 0x1000 $^ --oformat binary  #i686-elf-ld


ports.o: ports.c ports.h
	i686-elf-gcc -ffreestanding -c ports.c -o ports.o -std=gnu99 -Wall -Wextra

util.o: util.c util.h
	i686-elf-gcc -ffreestanding -c util.c -o util.o -std=gnu99 -Wall -Wextra

screen.o: screen.c screen.h
	i686-elf-gcc -ffreestanding -c screen.c -o screen.o -std=gnu99 -Wall -Wextra
	
kernel.o: kernel.c 
	i686-elf-gcc -ffreestanding -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra

kernel_entry.o: kernel_entry.s
	i686-elf-as -o kernel_entry.o kernel_entry.s

