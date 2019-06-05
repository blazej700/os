
#export PATH="$HOME/opt/cross/bin:$PATH" #exprot cross compiler prefix before make

os-image.bin: boot.bin kernel.bin
	cat $^ > os-image.bin
	rm *.o
boot.bin: boot.o
	i686-elf-ld -Ttext 0x7c00 --oformat binary -o boot.bin boot.o
boot.o: bootloader/boot.s
	i686-elf-as -o boot.o bootloader/boot.s

kernel.bin:kernel_entry.o kernel.o ports.o util.o screen.o descriptor_tables.o isr.o gdt.o interrupt.o timer.o keyboard.o input.o process.o yield.o floppy.o fat12.o
	i686-elf-ld -o kernel.bin -T linker.ld $^   #i686-elf-ld

interrupt.o: kernel/interrupt.s
	i686-elf-as -o interrupt.o kernel/interrupt.s

gdt.o: bootloader/gdt.s
	i686-elf-as -o gdt.o bootloader/gdt.s

input.o: drivers/input.c drivers/input.h 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c drivers/input.c -o input.o -std=gnu99 -Wall -Wextra

keyboard.o: drivers/keyboard.c drivers/keyboard.h 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c drivers/keyboard.c -o keyboard.o -std=gnu99 -Wall -Wextra

timer.o: drivers/timer.c drivers/timer.h 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c drivers/timer.c -o timer.o -std=gnu99 -Wall -Wextra

isr.o: kernel/isr.c kernel/isr.h 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c kernel/isr.c -o isr.o -std=gnu99 -Wall -Wextra

descriptor_tables.o: kernel/descriptor_tables.c kernel/descriptor_tables.h 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c kernel/descriptor_tables.c -o descriptor_tables.o -std=gnu99 -Wall -Wextra

ports.o: utils/ports.c utils/ports.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c utils/ports.c -o ports.o -std=gnu99 -Wall -Wextra

util.o: utils/util.c utils/util.h utils/types.h utils/stdarg.h drivers/input.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c utils/util.c -o util.o -std=gnu99 -Wall -Wextra

screen.o: drivers/screen.c drivers/screen.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c drivers/screen.c -o screen.o -std=gnu99 -Wall -Wextra
	
kernel.o: kernel/kernel.c kernel/kernel.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c kernel/kernel.c -o kernel.o -std=gnu99 -Wall -Wextra

kernel_entry.o: bootloader/kernel_entry.s
	i686-elf-as -o kernel_entry.o bootloader/kernel_entry.s

floppy.o: drivers/floppy.c 
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c drivers/floppy.c  -o floppy.o -std=gnu99 -Wall -Wextra

process.o: kernel/process.c kernel/process.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c kernel/process.c -o process.o -std=gnu99 -Wall -Wextra

yield.o: kernel/yield.s
	i686-elf-as -o yield.o kernel/yield.s

fat12.o: kernel/fat12.c kernel/fat12.h
	i686-elf-gcc -ffreestanding -nostdlib -lgcc -c kernel/fat12.c -o fat12.o -std=gnu99 -Wall -Wextra

bootf.bin: bootf.o
	i686-elf-ld -Ttext 0x7c00 --oformat binary -o bootf.bin bootf.o

bootf.o: toFatBootloader/boot.s
	i686-elf-as -o bootf.o toFatBootloader/boot.s

floppy:
	dd if=/dev/urandom of=os-image.img bs=512 count=2880
	dd if=os-image.bin of=os-image.img conv=notrunc

clean:
	rm *.o

clean-bin:
	rm *.bin

clean-all:
	rm *.o *.bin *.img

run: os-image.bin
	qemu-system-i386 -fda os-image.bin
