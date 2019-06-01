
#Wprowadzenie GDT

.global gdt_flush

gdt_flush:
    mov 4(%esp), %eax
    lgdt (%eax)
    jmp $0x08, $.flush

.flush:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

.global idt_flush
idt_flush:
    mov 4(%esp), %eax
    lidt (%eax)
    ret
