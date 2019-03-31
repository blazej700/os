#include "screen.h"
#include "util.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"

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

	init_descriptor_tables();

	irq_install();

	print_r("OS v1\n");
	print_r("Dzien dobry\n");
	print("To stop, type STOP.\nEverything eles will be printed.\nIt's sooooo usefull, have fun.");
	print("\n> ");

	for(;;);//zeby nie zakonczyc dzialania
 	
}

void user_input(char *input) {
    if (strcmp(input, "STOP") == 1) {
        print("Stopping. Bye!\n");
        asm volatile("hlt");
    }
    print("You said: ");
    print(input);
    print("\n> ");
}