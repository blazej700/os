#include "screen.h"
#include "util.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "input.h"
#include "types.h"

//Sprawdzamy czy na pewno nie kompilujemy dla os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
 
//Czy na pewno kompilujemy w 32bit
#if !defined(__i386__)
#error "This needs to be compiled with a i686-elf compiler"
#endif 

hour init_time;

void clock_print()
{
	hour time_now = get_time();

	time_now.s = time_now.s + init_time.s;
	time_now.m = time_now.m + init_time.m;
	time_now.h = time_now.h + init_time.h;

	if(time_now.s > 59){
		time_now.s=time_now.s-60;
	    time_now.m++;
	}
	if(time_now.m > 59){
	    time_now.m=time_now.m-60;
	    time_now.h++;
	}
	
	char se[3];
	char mi[3];
	char ho[3];
	int_to_ascii(time_now.s,se);
	int_to_ascii(time_now.m,mi);
	int_to_ascii(time_now.h,ho);
	print(ho);
	print(":");
	print(mi);
	print(":");
	print(se);
	
}

void clock_init()
{
	char input[15];

	print("\nCurent time.\n");

	print("H:");
	scan_c(input);
	init_time.h = str_to_int(input);

	print("M:");
	scan_c(input); 
	init_time.m = str_to_int(input);

	print("S:");
	scan_c(input);
	init_time.s = str_to_int(input);

	char se[3];
	char mi[3];
	char ho[3];
	int_to_ascii(init_time.s,se);
	int_to_ascii(init_time.m,mi);
	int_to_ascii(init_time.h,ho);
	print(ho);
	print(":");
	print(mi);
	print(":");
	print(se);
	print("\n");
	
	time_clear();
}

void malloc_test()
{
	char size_input[10];
	print("Size to allocate");
	print("\nMALLOC> ");
	scan_c(size_input);

	u32int size;
	size = (u32int)(str_to_int(size_input));

	int_to_ascii(size, size_input);
	print(size_input);
	print("\n");

	u32int phys_addr;
	u32int page = kmalloc(size, &phys_addr);
	char page_str[16] = "";
    hex_to_str(page, page_str);
	char phys_str[16] = "";
	hex_to_str(phys_addr, phys_str);
	print("Page: ");
	print(page_str);
	print(", physical address: ");
	print(phys_str);
	print("\n");
}

void main()
{
	clear_s();

	init_descriptor_tables();

	irq_install();

	print_r("OS v1\n");
	print_r("Dzien dobry\n");
	print("To stop, type STOP.\n");
	print("To set  time, type SETTIME.\n");
	print("To curent time, type TIME.\n");
	print("To kmalloc, type MALLOC.\n");
	print("Everything else will be printed.\n");
	print("It's sooooo useful, have fun.\n");

	char input[256];

	while(1)
	{
		print("\n> ");
		scan_c(input);
		if (strcmp(input, "STOP") == 1) 
		{
	    	print("Stop");
	    	asm volatile("cli");
	        asm volatile("hlt");
	    }
	    else if(strcmp(input, "TIME") == 1)
	    {
	    	clock_print();
	    }
	    else if(strcmp(input, "MALLOC") == 1)
	    {
	    	malloc_test();
	    }
	    else if(strcmp(input, "SETTIME") == 1)
	    {
	    	clock_init();
	    }
	    else
		{	    
			print("You said: ");
		    print(input);
		}
	}

	for(;;);//zeby nie zakonczyc dzialania
 	
}

