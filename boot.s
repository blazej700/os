.code16      


.bss
.comm boot_drive, 1  

.text
 
KERNEL_OFFSET = 0x1000

.global _start 
_start: 

	mov $0, %eax
	mov %dx, boot_drive(, %eax, 1)

	mov $0x9000, %bp
	mov %bp, %sp
  
	mov $bit16_msg, %edx
	mov $bit16_msg_l, %ecx
	call print
	call print_nl

	call load_kernel
	call switch_to32
	jmp .
  
	.include "string_16print.s"
	.include "hex_16print.s"
	.include "load_disk.s"
	#.include "gdt.s"
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

	mov $1, %dh #pamietac zeby zwiekszyc jak braknie miejsca xda 
	mov boot_drive(, %eax, 1), %dl
	
	call disk_load
	ret

  
.code32
start_32:
	
	movl $bit32_msg, %ebx
	movl $bit32_msg_l, %ecx
	call string_32print
	call KERNEL_OFFSET

bit16_msg: .ascii "in 16-bit"
bit16_msg_l = . - bit16_msg
ker_load_msg: .ascii "loading kernel"
ker_load_msg_l = . - ker_load_msg
bit32_msg: .ascii "finally in 32-bit"
bit32_msg_l = . - bit32_msg

. = _start + 510     #mov to 510th byte from 0 pos
     .byte 0x55           #append boot signature
     .byte 0xaa           #append boot signature
