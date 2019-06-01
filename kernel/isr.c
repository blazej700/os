#include "isr.h"

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
bool irq_fired[256];

// To wywolujemy w naszym kodzie asemblera gdy mamy dany wyjatek, kody bledow sa w rejestrach
void __attribute__((aligned(16))) isr_handler(u32int ds, CPUState regs)
{
    print_f("received interrupt: %d\n", regs.int_no);
    print_f("error code:         %d\n", regs.err_code);
    print_f("eip:         %x\n\n", regs.eip);
}

void register_interrupt_handler(u8int n, isr_t handler) 
{
    interrupt_handlers[n] = handler;
}

u32int __attribute__((aligned(16))) irq_handler(u32int int_no, u32int esp)
{
    irq_fired[int_no] = true;
    if (interrupt_handlers[int_no] != 0) 
    {
        isr_t handler = interrupt_handlers[int_no];
        //handlery muszą zwracaś esp żeby przywrócić stan procesora
        //można po prostu zwrócić ten sam esp który przyjmuje jako argument
        //w praktyce tylko handler timera zmienia esp
        esp = handler(esp);
    }
    //Odswiezenie, wysylamni EOI do PIC'ow zeby odblokowac przerwania
    if (int_no >= 40)
    {   
        port_byte_out(0xA0, 0x20); // slave
    }
    port_byte_out(0x20, 0x20); // master 
    return esp;
}

void irq_install()
{

	//aktywowanie przerwan
    asm volatile("sti");
    
    //IRQ1 klawiatura
	init_keyboard();

    //Musi być ostatnie bo uruchamia wielowątkowość
    //IRQ0 timer
    init_timer(100);
}

void irq_wait(int irq)
{
    irq_fired[irq] = false;
    while(1) 
    {
        if(!irq_fired[irq])
            return;
    }
}