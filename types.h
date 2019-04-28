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

typedef struct hour_st
{
	u8int h;
	u8int m;
	u8int s;
} hour;


#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif