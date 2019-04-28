#include "timer.h"
#include "isr.h"
#include "screen.h"
#include "util.h"
#include "ports.h"

u32int tick = 0;
hour time;

hour get_time()
{
   return time;
}
void time_clear()
{
   time.h = 0;
   time.m = 0;
   time.s = 0;
}

static void timer_callback(registers_t regs)
{
   //na razie to tylko liczy i nic nie robi, ale potem moze sie przydac
   tick++;
 
   if(tick%100 == 0)
   {

      time.s++;

      if(time.s>59)
      {
         time.s=0;
         time.m++;
      }

   }
   
}

void init_timer(u32int frequency)
{
   time.h = 0;
   time.m = 0;
   time.s = 0;
   //dodanie do IRQ0 timera
   register_interrupt_handler(IRQ0, &timer_callback);

 
   u16int divisor = 1193180 / frequency;

   port_byte_out(0x43, 0x36);


   u8int low = (u8int)(divisor & 0xFF);
   u8int high = (u8int)( (divisor>>8) & 0xFF );


   port_byte_out(0x40, low);
   port_byte_out(0x40, high);
} 