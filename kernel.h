#ifndef KERNEL_H
#define KERNEL_H

#include "screen.h"
#include "util.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "input.h"
#include "types.h"
#include "process.h"
#include "floppy.h"

hour init_time;

void clock_print();

void clock_init();

void malloc_test();

void floppy_test();

void system_menu();

void main();

#endif