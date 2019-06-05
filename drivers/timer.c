#include "timer.h"

u32int tick = 0;
hour time;

const int scheduler_interval = 5;
int scheduler_current_interval = scheduler_interval;

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

static __attribute__((aligned(16))) u32int timer_callback(u32int esp)
{
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

   int i;
   for(i=0; i<143; i++)
   {
      if(task_manager.tasks[i].status == SLEEPING_PROCESS)
      {
         if(tick >= task_manager.tasks[i].timer_ticks)
            task_manager.tasks[i].status = ACTIVE_PROCESS;
      }
   }

   scheduler_current_interval--;
   if(scheduler_current_interval <= 0) 
   {
      scheduler_current_interval = scheduler_interval;
      //każ schedulerowi zmienić proces
      esp = schedule((CPUState *)esp); 
   }
   return esp;
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

void sleep(int sleep_ticks)
{  
   task_manager.tasks[task_manager.current_task].timer_ticks = tick + sleep_ticks;
   task_manager.tasks[task_manager.current_task].status = SLEEPING_PROCESS;
   yield();
}