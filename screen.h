#ifndef SCREEN_H
#define SCREEN_H

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

#endif