#ifndef TIMER_H
#define TIMER_H

#include "../utils/types.h"
#include "screen.h"
#include "../utils/util.h"
#include "../utils/ports.h"
#include "../kernel/process.h"

void init_timer(u32int frequency);
hour get_time();
void time_clear();
void sleep(int ticks);
extern void yield();

#endif