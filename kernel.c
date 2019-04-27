#include "screen.h"
#include "util.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"

//Sprawdzamy czy na pewno nie kompilujemy dla os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
 
//Czy na pewno kompilujemy w 32bit
#if !defined(__i386__)
#error "This needs to be compiled with a i686-elf compiler"
#endif 


void main()
{
	clear_s();

	init_descriptor_tables();

	irq_install();

	print_r("OS v1\n");
	print_r("Dzien dobry\n");
	print("To stop, type STOP.\nEverything else will be printed.\nIt's sooooo useful, have fun.");
	print("\n> ");

	for(;;);//zeby nie zakonczyc dzialania
 	
}

void user_input(char *input) 
{
    
    if (strcmp(input, "STOP") == 1) 
    {
        print("Stopping. Bye!\n");
        asm volatile("hlt");
    }
    
    print("You said: ");
    print(input);
    print("\n> ");
}