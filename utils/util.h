#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "../drivers/screen.h"
#include "stdarg.h"
#include "../drivers/input.h"


#define PRINT_BUF_LEN 100

void memory_copy(char *source, char *dest, int nbytes);
void memory_shift(char *source, int len, int offset);
void int_to_ascii(int n, char str[]);
void float_to_ascii(float n, char str[], int prec);
void str_format_var(char *buf, char *format, va_list args);
void str_format(char *buf, char *format, ...);
void print_f(char *format, ...);
int str_read_format_var(char *buf, char *format, va_list args);
int str_read_format(char *buf, char *format, ...);
int scan_f(char *format, ...);
int strl(char* s);
void memory_set(u8int *dest, u8int val, u32int len);
void append(char s[], char n);
void delend(char s[]);
int strcmp(char s1[], char s2[]);
int str_copy(char a[], char b[]);
void rev(char str[], int len);
void swap(char *a1, char *a2);
int str_to_int(char s[]);
float str_to_float(char s[]);
u32int hex_str_to_int(char s[]);
int pow(int a, int b);
void hex_to_str(u32int hex, char str[]);
int is_letter(char c);
int is_digit(char c);
int is_whitespace(char c);
u32int kmalloc(u32int size, u32int *phys_addr);


#endif
