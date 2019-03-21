
os-image.bin: boot.bin kernel.bin
	cat $^ > os-image.bin
boot.bin: boot.o
	ld -Ttext 0x7c00 --oformat binary -o boot.bin boot.o
boot.o: boot.s
	as -o boot.o boot.s
kernel.bin: kernel_entry.o kernel.o
	ld -o kernel.bin -Ttext 0x1000 $^ --oformat binary 

kernel.o: kernel.c 
	gcc -ffreestanding -c kernel.c -o kernel.o
kernel_entry.o: kernel_entry.s
	as -o kernel_entry.o kernel_entry.s

