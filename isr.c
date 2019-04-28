#include "isr.h"
#include "screen.h"
#include "util.h"
#include "timer.h"
#include "keyboard.h"
#include "ports.h"


//Nazwy wyjatkow
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

isr_t interrupt_handlers[256];


// To wywolujemy w naszym kodzie asmeblera gdy mamy dany wyjatek, kody bledow sa w rejestrach
void isr_handler(registers_t regs)
{
    print("received interrupt: ");
    char s[3];
    int_to_ascii(regs.int_no, s);
    print(s);
    print("\n");
    print(exception_messages[regs.int_no]);
    print("\n");
}

void register_interrupt_handler(u8int n, isr_t handler) 
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) 
{

	//Odswiezenie, wysylamni EOI do PIC'ow zeby odblokowac przerwania
    if (r.int_no >= 40)
    {
        port_byte_out(0xA0, 0x20); // slave
	}
    port_byte_out(0x20, 0x20); // master 

    if (interrupt_handlers[r.int_no] != 0) 
    {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_install()
{

	//aktywowanie przerwan
    asm volatile("sti");
    
    //IRQ0 timer
    init_timer(100);

    //IRQ1 klawiatura
	init_keyboard();

}