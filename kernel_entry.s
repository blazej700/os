.code32
.text

.global kstart
kstart:
    movl $0x900000, %esp
    movl %esp, %ebp
	call main #wywolanie naszego jadra w C
	
	cli
j1:	
	hlt
	jmp j1    
            
