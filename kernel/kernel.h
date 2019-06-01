#ifndef KERNEL_H
#define KERNEL_H

#include "../drives/screen.h"
#include "../utils/util.h"
#include "descriptor_tables.h"
#include "../drives/timer.h"
#include "../drives/input.h"
#include "../utils/types.h"
#include "process.h"
#include "../drives/floppy.h"

hour init_time;

void clock_print();

void clock_init();

void malloc_test();

void floppy_test();

void system_menu();

void main();

#endif