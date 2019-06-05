#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../utils/types.h"
#include "../utils/ports.h"
#include "../kernel/isr.h"
#include "../drivers/screen.h"
#include "../drivers/input.h"

void init_keyboard();

#endif