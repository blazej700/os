#ifndef TIMER_H
#define TIMER_H

#include "types.h"
#include "screen.h"
#include "util.h"
#include "ports.h"
#include "process.h"

void init_timer(u32int frequency);
hour get_time();
void time_clear();
void sleep(int ticks);
extern void yield();

#endif