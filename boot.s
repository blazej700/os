.code16      

.bss
.comm boot_drive, 1  #do zapisania numeru naszeego dysku

.text
 
KERNEL_OFFSET = 0x1000	#adres od ktorego zapiszemy jadro w pamieci

.global _start 
_start: 

	mov $0, %eax
	mov %dx, boot_drive(, %eax, 1)	#zapisanie numeru naszego dysku do pamieci zeby go nie zgubic

	mov $0x9000, %bp	#ustawienie stosu, os za nas tego nie zrobi...
	mov %bp, %sp
  
	mov $bit16_msg, %edx		#wyswietlenie ze jestesmy w rm mode
	mov $bit16_msg_l, %ecx
	call print
	call print_nl

	call load_kernel	#ladujemy nasze jadro z dysku do pamieci
	call switch_to32 	#przelaczamy sie w pm mode
	jmp . 				#gdyby poprzednie instrukcje sie nie wykonaly, to tutaj sie zatrzyamamy, ale jak wszystko jest ok
						#to nigdy tu nie dojdziemy
  
	.include "string_16print.s"
	.include "hex_16print.s"
	.include "load_disk.s"
	.include "string_32print.s"
	.include "switch_to32.s"

.code16
load_kernel:
	mov $ker_load_msg, %edx
	mov $ker_load_msg_l, %ecx
	call print
	call print_nl

	mov $0, %eax
 
	mov $KERNEL_OFFSET, %bx

	mov $31, %dh 					#ilosc sektorow do wczytania, pamietac zeby zwiekszyc jak braknie miejsca xda 
	mov boot_drive(, %eax, 1), %dl
	
	call disk_load	#wywolanie wczytywania z dysku
	ret

  
.code32
start_32:
	
	movl $bit32_msg, %ebx	#wyswietlamy informacje ze jestesmy w 32 pm mode
	movl $bit32_msg_l, %ecx
	call string_32print
	call KERNEL_OFFSET	#przechodzimy do adresu w ktorym zaczyna sie nasze jadro

bit16_msg: .ascii "in 16-bit"
bit16_msg_l = . - bit16_msg
ker_load_msg: .ascii "loading kernel"
ker_load_msg_l = . - ker_load_msg
bit32_msg: .ascii "finally in 32-bit"
bit32_msg_l = . - bit32_msg

. = _start + 510     #przesuwamy sie 510 bajtow od poczatku
     .byte 0x55           #dwa ostanie bajty 
     .byte 0xaa           #wpisujemy magiczana liczbe 0x55aa
