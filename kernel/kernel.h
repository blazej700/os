#ifndef KERNEL_H
#define KERNEL_H

#include "../drivers/screen.h"
#include "../utils/util.h"
#include "descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/input.h"
#include "../utils/types.h"
#include "process.h"
#include "../drivers/floppy.h"

hour init_time;

void clock_print();

void clock_init();

void malloc_test();

void floppy_test();

void system_menu();

void main();

#endif