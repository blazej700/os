.code32
.text

.global _start
start:
	call main 
	cli
j1:	
	hlt
	jmp j1   
            