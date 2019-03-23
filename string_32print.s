.code32

VIDEO_M = 0xb8000 #adres pamieci grafiki

COLOR_WB = 0b00000010 #atrybuty, formak bbbb|cccc b-kolor_tlo c-kolor_text

string_32print:	#adres do naszego napisu bedzie w %ebx, dlugosc w %ecx
 	pusha
 	mov $VIDEO_M, %edx	

loop_string_32print:
 	mov (%ebx), %al
 	#mov $'a', %al 	
 	mov $COLOR_WB, %ah
 
 	mov %ax, (%edx)
 	inc %ebx
 	add $2, %edx

 	loop loop_string_32print

 	popa
 	ret
