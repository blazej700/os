.code16
switch_to32:
	cli

	movw $0, %ax
	movw %ax, %ds

	#tablica deskryptorw

	movl $0x00000000, 0x800
	movl $0x00000000, 0x804
	movl $0x0000FFFF, 0x808 # Data segment descriptor
	movl $0x00CF9200, 0x80C # read/write
	movl $0x0000FFFF, 0x810 # Code segment descriptor
	movl $0x00CF9800, 0x814 # execute/read

	lgdt gdt_reg

	movl %cr0, %eax
	or $0x01, %al
	movl %eax, %cr0

	jmp $0x10, $init_32 #daleki skok do init_32, po wykonaniu jestesmy w pm mode

gdt_reg:
	.word 0x0017
	.long 0x00000800

.code32
init_32:
 	movw $0x8, %ax # We set up %ds and %ss pointing on the Data segment
	movw %ax, %ds
	movw %ax, %ss
	call start_32

