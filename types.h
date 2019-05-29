#ifndef TYPES_H
#define TYPES_H

/* 
 * Definiujemy sobie bardzo przydatne typy, zeby nie uzywac wszedzie char'ow bo sa brzydkie 
 */
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;
typedef enum { false, true } bool;

typedef struct hour_st
{
	u8int h;
	u8int m;
	u8int s;
} hour;

typedef volatile struct
{
    u32int eax;
    u32int ebx;
    u32int ecx; 
    u32int edx;

    u32int esi;
    u32int edi;
    u32int ebp;

    u32int int_no;
    u32int err_code;

    u32int eip;
    u32int cs;
    u32int eflags;
    u32int esp;
    u32int ss;
} CPUState __attribute__((aligned(16)));

typedef enum
{
    SLEEPING_PROCESS = 0, 
    ACTIVE_PROCESS   = 1, 
    DEAD_PROCESS     = 2
} process_status;

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif