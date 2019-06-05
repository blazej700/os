.extern schedule

.global yield
yield:    
    cli
    movl %eax, -12(%esp)
    movl %ebx, -16(%esp)
    popl %ebx #zapisz eip

    pushfl
    mov %cs, %ax
    pushl %eax
    pushl %ebx #przywroc eip na stos

    movl -4(%esp), %eax
    movl -8(%esp), %ebx

    pushl $0
    pushl $0
    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    pushl %esp

    call schedule

    mov %eax, %esp
    
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    add $8, %esp
    sti
    iret
