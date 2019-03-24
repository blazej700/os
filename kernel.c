#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "screen.h"
#include "util.h"

//Sprawdzamy czy napewno nie komilujemy dla os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
 
//Czy napewno kompilujemy w 32bit
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif 



void main()
{
	clear_s();
	int i;
	for(i=0; i<23; i++)
	{

		print_r("a");
		print_r("\n");
	}

	print("defgh\n");
	print("tralalal\n");
	print_r("defgh");

}
