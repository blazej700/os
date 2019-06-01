#ifndef SCREEN_H
#define SCREEN_H

#include "../utils/ports.h"
#include "../utils/util.h"

#define VIDEO_ADDRESS 0xb8000	//adres poczatku pamieci vga
#define MAX_ROWS 25	
#define MAX_COLS 80
#define DEFAULT_COLOR 0x02      //zielony tekst na czarnym tle


// Porty i/o wyswietlania
#define REG_SCREEN_CTRL 0x3d4	
#define REG_SCREEN_DATA 0x3d5


void clear_s();
void print_at(char *msg, int col, int row, char color);
void print(char *msg);
void print_r(char *msg);
void remove_last();
int print_c(char a, int col, int row, char color);
int scroll();
int get_cursor();
void set_cursor(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);


#endif