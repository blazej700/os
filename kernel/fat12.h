#ifndef FAT12_H
#define FAT12_H

#include "../drivers/screen.h"
#include "../utils/util.h"
#include "descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/input.h"
#include "../utils/types.h"
#include "process.h"
#include "../drivers/floppy.h"

#define first_fat_sector 19
#define fat_size 32

#define first_data_sector 33
#define cluster_inFat_offset 26
#define size_offset 28
#define cluster_size 2
#define size_size 3
#define max_fats 112

#define cluster_offset 27

#define name_offset 0
#define name_size 8
#define ext_offset 8
#define ext_size 3

#define date_size 2
#define date_offset 24


typedef struct fat12_st{
	char name[9];
	char ext[4];
	unsigned int cluster;
	unsigned int size;
}fat_file;

void print_files();
void dump_file(char name_1[], bool hex);
void open_program(char name_1[]);
void write_to_file(char name_1[], char values[]);


#endif