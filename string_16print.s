

print:  #wyswietlanie napisu 
		#w %edx adres tego co mamy wyswietlic, w %ecx dlugosc
	pusha

	l1:
		mov $0xe, %ah
		mov (%edx), %al
		int $0x10
		inc %edx
	loop l1

	popa
	ret

print_nl:	#printing new line
	pusha

	mov $0x0e, %ah
	mov $0x0a, %al
	int $0x10
	mov $0x0d, %al
	int $0x10

	popa
	ret
