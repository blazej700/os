

print_hexb:     #wyswietlanie liczby hex 2 bajtowej
				#w %dx mamy wartosc do wyswietlenia
	
	#0xffff
	pusha
	mov $0xe, %ah
	mov $4, %ecx

	mov $'0', %al
	int $0x10
	mov $'x', %al
	int $0x10

	l2:
		mov %dh, %al

		and $0xf0, %al
		shr $4, %al

		cmp $0x9, %al #if(0x9 < %al){jmp is_0} else {jmp is_a}
		jl is_0
		jmp is_a
		is_a:
			add $0x37, %al
			jmp if1_end
		is_0:
			add $'0', %al
			jmp if1_end
		if1_end:
		int $0x10
		shl $4, %dx
	loop l2
	popa
ret
