
disk_load:
	pusha
	push %dx #tam jest numer naszego dysku

	mov $0x02, %ah  #w int 0x13 0x02 to funkcja read
	mov %dh, %al
	mov $0x02, %cl

	mov $0x00, %ch

	mov $0x00, %dh

	int $0x13

	jc disk_error

	pop %dx
	cmp %dh, %al

	jne disk_error_sector

	popa
	ret

disk_error:
	mov $disk_error_msg, %edx
	mov $disk_error_msg_l, %ecx
	call print
	call print_nl
	mov %ax, %dx			#ZREWIZOWAĆ! już jest chybaxd
	call print_hexb
	jmp disk_loop

disk_error_sector:
	mov $disk_error_sector_msg, %edx
	mov $disk_error_sector_msg_l, %ecx
	call print

disk_loop:
	jmp disk_loop

disk_error_msg: .ascii "disk loading can not be executed"
disk_error_msg_l = . - disk_error_msg
disk_error_sector_msg: .ascii "number of sectors to read is not correct"
disk_error_sector_msg_l = . - disk_error_sector_msg

