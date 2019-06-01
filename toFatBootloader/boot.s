.code16      

p0:


jmp _start

.byte 0x90 

.byte 0x4d 
.byte 0x53
.byte 0x57 
.byte 0x49
.byte 0x4e 
.byte 0x34
.byte 0x2e 
.byte 0x31
.byte 0x00 
.byte 0x02
.byte 0x01 
.byte 0x01
.byte 0x00
.byte 0x02
.byte 0xe0
.byte 0x00
.byte 0x40 
.byte 0x0b
.byte 0xf0 
.byte 0x09
.byte 0x00
.byte 0x12
.byte 0x00 
.byte 0x02
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x29
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x4e
.byte 0x4f
.byte 0x20 
.byte 0x4e
.byte 0x41
.byte 0x4d
.byte 0x45 
.byte 0x20
.byte 0x20 
.byte 0x20
.byte 0x20
.byte 0x46
.byte 0x41 
.byte 0x54
.byte 0x31 
.byte 0x32
.byte 0x20 
.byte 0x20
.byte 0x20

p2:

.bss
.comm boot_drive, 1  #do zapisania numeru naszego dysku

.text
 
KERNEL_OFFSET = 0x1000	#adres od ktorego zapiszemy jadro w pamieci

.global _start 
_start: 

	mov $0, %eax
	mov %dx, boot_drive(, %eax, 1)	#zapisanie numeru naszego dysku do pamieci zeby go nie zgubic

	mov $0x9000, %bp		#ustawienie stosu, os za nas tego nie zrobi...
	mov %bp, %sp
  
	mov $bit16_msg, %edx		#wyswietlenie ze jestesmy w real mode
	mov $bit16_msg_l, %ecx
	call print
	call print_nl

	call load_kernel		#ladujemy nasze jadro z dysku do pamieci
	call switch_to32 		#przelaczamy sie w protected mode
	jmp . 				#gdyby poprzednie instrukcje sie nie wykonaly, to tutaj sie zatrzymamy, ale jak wszystko jest ok
					#to nigdy tu nie dojdziemy
  
	.include "toFatBootloader/string_16print.s"
	.include "toFatBootloader/hex_16print.s"
	.include "toFatBootloader/load_disk.s"
	.include "toFatBootloader/string_32print.s"
	.include "toFatBootloader/switch_to32.s"

.code16
load_kernel:
	mov $ker_load_msg, %edx
	mov $ker_load_msg_l, %ecx
	call print
	call print_nl

	mov $0, %eax
 
	mov $KERNEL_OFFSET, %bx

	mov $43, %dh 			#ilosc sektorow do wczytania, pamietac zeby zwiekszyc jak braknie miejsca xda 
	mov boot_drive(, %eax, 1), %dl
	
	call disk_load			#wywolanie wczytywania z dysku
	ret

  
.code32
start_32:
	
	movl $bit32_msg, %ebx	#wyswietlamy informacje ze jestesmy w protected mode
	movl $bit32_msg_l, %ecx
	call string_32print
	call KERNEL_OFFSET	#przechodzimy do adresu w ktorym zaczyna sie nasze jadro

bit16_msg: .ascii "in16bit"
bit16_msg_l = . - bit16_msg
ker_load_msg: .ascii "loadingKernel"
ker_load_msg_l = . - ker_load_msg
bit32_msg: .ascii "in32bit"
bit32_msg_l = . - bit32_msg

. = p0 + 510     		#przesuwamy sie 510 bajtow od poczatku
     .byte 0x55         	#dwa ostatnie bajty 
     .byte 0xaa         	#wpisujemy magiczna liczbe 0x55aa
