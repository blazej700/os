#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "util.h"

u32int tick = 0;

static void timer_callback(registers_t regs)
{
   //narazie to tylko liczy i nic nierobi, ale potem moze sie przydac
   tick++;
}

void init_timer(u32int frequency)
{
   //dodanie do IRQ0 timera
   register_interrupt_handler(IRQ0, &timer_callback);

 
   u32int divisor = 1193180 / frequency;

   port_byte_out(0x43, 0x36);


   u8int low = (u8int)(divisor & 0xFF);
   u8int high = (u8int)( (divisor>>8) & 0xFF );


   port_byte_out(0x40, low);
   port_byte_out(0x40, high);
} 