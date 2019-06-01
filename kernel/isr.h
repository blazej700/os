#ifndef ISR_H
#define ISR_H

#include "../utils/types.h"
#include "../drives/screen.h"
#include "../utils/util.h"
#include "../drives/timer.h"
#include "../drives/keyboard.h"
#include "../utils/ports.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef u32int (*isr_t)(u32int);
void isr_handler(u32int ds, CPUState regs);
void register_interrupt_handler(u8int n, isr_t handler);
u32int irq_handler(u32int int_no, u32int esp);
void irq_install();
void irq_wait(int irq);

#endif