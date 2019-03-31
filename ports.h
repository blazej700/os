#ifndef PORTS_H
#define PORTS_H

#include "types.h"

u8int port_byte_in (u16int port);
void port_byte_out (u16int port, u8int data);
u16int port_word_in (u16int port);
void port_word_out (u16int port, u16int data);

#endif