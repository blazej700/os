#include "ports.h"

/**
 * czyta i zapisuje z podanego portu
 */
u8int port_byte_in (u16int port) {
    u8int result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out (u16int port, u8int data) {

     
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}

u16int port_word_in (u16int port) {
    u16int result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out (u16int port, u16int data) {
    __asm__ __volatile__("out %%ax, %%dx" : : "a" (data), "d" (port));
}