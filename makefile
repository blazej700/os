
#export PATH="$HOME/opt/cross/bin:$PATH" #exprot cross compiler prefix before make

os-image.bin: boot.bin kernel.bin
	cat $^ > os-image.bin
boot.bin: boot.o
	i686-elf-ld -Ttext 0x7c00 --oformat binary -o boot.bin boot.o
boot.o: boot.s
	i686-elf-as -o boot.o boot.s
kernel.bin:kernel_entry.o kernel.o ports.o util.o screen.o descriptor_tables.o isr.o gdt.o interrupt.o timer.o keyboard.o
	i686-elf-ld -o kernel.bin -Ttext 0x1000 $^ --oformat binary  #i686-elf-ld

interrupt.o: interrupt.s
	i686-elf-as -o interrupt.o interrupt.s

gdt.o: gdt.s
	i686-elf-as -o gdt.o gdt.s

keyboard.o: keyboard.c keyboard.h 
	i686-elf-gcc -ffreestanding -c keyboard.c -o keyboard.o -std=gnu99 -Wall -Wextra

timer.o: timer.c timer.h 
	i686-elf-gcc -ffreestanding -c timer.c -o timer.o -std=gnu99 -Wall -Wextra

isr.o: isr.c isr.h types.h
	i686-elf-gcc -ffreestanding -c isr.c -o isr.o -std=gnu99 -Wall -Wextra

descriptor_tables.o: descriptor_tables.c descriptor_tables.h types.h
	i686-elf-gcc -ffreestanding -c descriptor_tables.c -o descriptor_tables.o -std=gnu99 -Wall -Wextra

ports.o: ports.c ports.h
	i686-elf-gcc -ffreestanding -c ports.c -o ports.o -std=gnu99 -Wall -Wextra

util.o: util.c util.h types.h
	i686-elf-gcc -ffreestanding -c util.c -o util.o -std=gnu99 -Wall -Wextra

screen.o: screen.c screen.h
	i686-elf-gcc -ffreestanding -c screen.c -o screen.o -std=gnu99 -Wall -Wextra
	
kernel.o: kernel.c kernel.h
	i686-elf-gcc -ffreestanding -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra

kernel_entry.o: kernel_entry.s
	i686-elf-as -o kernel_entry.o kernel_entry.s

