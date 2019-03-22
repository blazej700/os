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

void dummy_test_entrypoint() 
{
}

void print_g()
{ 
	char napis[]={'h','e','j'};
	char* video_memory = (char*) 0xb8000;
	*(video_memory+1)= 0xf;
	int i=0;
	for(i; i<3; i++){
		*video_memory = napis[i];
		video_memory=video_memory+2;    
	}
 	
 
}

void main()
{
	char* video_memory = (char*) 0xb8000;
	print_g();
	*video_memory='a';
}
