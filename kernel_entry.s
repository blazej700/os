.code32
.text

.global _start
start:
	call main #wywolanie naszego jadra w C
	
	cli
j1:	
	hlt
	jmp j1    
            
