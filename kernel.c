#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif 

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void term_cl()
{
	char* video_memory = (char*) 0xb8000;
	for(int i=0; i<2000; i++)
	{
		*video_memory = 0x0;
		video_memory++;
	}

}

void print_g()
{ 
	char* napis="Ok, to jest dlugi tekst, ciekawe jak wyglada, poogladajmy sobie eee                            2000 literek to calkiem duzo xdddddd, deabak, naneun Jong Ye-rin sieul saranghaeyo";
	char* video_memory = (char*) 0xb8000;

	size_t i=0;
	size_t j=strlen(napis);

	for(i; i<j; i++){
		*video_memory = napis[i];
		//*(video_memory+1) = 0b00000000;
		video_memory++;
		*video_memory=0b00000100;
		video_memory++;    
	}
}

void main()
{
	term_cl();
	print_g();

}
