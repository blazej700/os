#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "../utils/util.h"
#include "../drivers/screen.h"
#include "../utils/types.h"
#include "../utils/ports.h"

void init_descriptor_tables();

//wartosci GDT entry
struct gdt_entry_struct
{
    u16int limit_low;           // 16 limit
    u16int base_low;            // 16 base
    u8int  base_middle;         // 8 base
    u8int  access;              // flaga dostepu
    u8int  granularity;
    u8int  base_high;           // 8 base.
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

// wskaznik na GDT 
struct gdt_ptr_struct
{
    u16int limit;               // elector limits
    u32int base;                // adres pierwszej gdt_entry_t
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

//interrupt gate, czyli to co wprowadza w dane miejsce jak jest przerwanie
struct idt_entry_struct
{
    u16int base_lo;             // dolne 16 bitu adresu...
    u16int sel;                 // segment selector
    u8int  always0;             // zero
    u8int  flags;               // flagi
    u16int base_hi;             // ...gorne 16 adresu gdzie mamy sie udac gdy wystapi przerwanie
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;


// Struktura opisuje wskaznik na tablice przerwan
struct idt_ptr_struct
{
    u16int limit;
    u32int base;                // adres pierwszego elementu w tablicy idt_entry_t
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

//Funcje z aseblera
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif