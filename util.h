#ifndef UTIL_H
#define UTIL_H

#include "types.h"

void memory_copy(char *source, char *dest, int nbytes);
void int_to_ascii(int n, char str[]);
int strl(char* s);
void memory_set(u8int *dest, u8int val, u32int len);
void append(char s[], char n);
void delend(char s[]);
int strcmp(char s1[], char s2[]);


#endif