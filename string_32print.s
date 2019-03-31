.code32

VIDEO_M = 0xb8000 		#adres pamieci grafiki

COLOR_WB = 0b00000010 		#atrybuty, format bbbb|cccc b-kolor_tlo c-kolor_text

#Funkcja wypisujaca napis w trybie protected

#----------------------------------
# %ebx - adres napisu
# %ecx - dlugosc napisu
#----------------------------------
string_32print:	
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
