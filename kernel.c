#include "kernel.h"

//Sprawdzamy czy na pewno nie kompilujemy dla os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
 
//Czy na pewno kompilujemy w 32bit
#if !defined(__i386__)
#error "This needs to be compiled with a i686-elf compiler"
#endif 

void scheduler_test()
{
	while(true)
	{
		print_f("Proces numer %d \n", task_manager.current_task);
		//print_f("przed: %x %x\n", task_manager.tasks[task_manager.current_task].regs->ss, task_manager.tasks[task_manager.current_task].regs->esp);
		sleep(500);
		//print_f("po: %x %x\n", task_manager.tasks[task_manager.current_task].regs->ss, task_manager.tasks[task_manager.current_task].regs->esp);
	}
}

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

	print_f("%02d:%02d:%02d", time_now.h, time_now.m, time_now.s);
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

	print_f("%02d:%02d:%02d", init_time.h, init_time.m, init_time.s);

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

	print_f("Page: %x, physical address: %p\n", page, phys_addr);
}

void floppy_test()
{
	int i=0;
	char floppy_input[10];
	// Paskudny hack, zeby nie wracalo do menu
	task_manager.tasks[0].status = DEAD_PROCESS;
	print("Which sector: ");
	scan_c(floppy_input);

	u32int start;
	start = (u32int)(str_to_int(floppy_input));

	print("How many bytes: ");
	scan_c(floppy_input);
	task_manager.tasks[0].status = ACTIVE_PROCESS;

	u32int size;
	size = (u32int)(str_to_int(floppy_input));

	floppy_read_track(start, size);
	for(i=0; i<size; i++)
		print_f("%d ",  floppy_dmabuf[i]);
}

void system_menu()
{
	print_r("OS v1\n");
	print_r("Dzien dobry\n");
	print("To stop, type STOP.\n");
	print("To set  time, type SETTIME.\n");
	print("To curent time, type TIME.\n");
	print("To kmalloc, type MALLOC.\n"); 
	print("To test scheduler, type SCHEDULER.\n"); 
	print("To list processes, type PS.\n"); 
	print("To kill process, type KILL.\n"); 
	print("To test floppy, type FLOPPY.\n"); 
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
	    else if(strcmp(input, "SCHEDULER") == 1)
	    {
			add_task(scheduler_test);
	    }
	    else if(strcmp(input, "PS") == 1)
	    {
	    	list_processes();
	    }
	    else if(strcmp(input, "KILL") == 1)
	    {
	    	char id_c[10];
	    	print("Process to kill");
			print("\n ");
			scan_c(id_c);
			int id;
			id = str_to_int(id_c);
			kill_task(id);
	    }
	    else if(strcmp(input, "FLOPPY") == 1)
	    {
			add_task(floppy_test);
	    }
	    else
		{	    
			print("You said: ");
		    print(input);
		}
	}

	for(;;);//zeby nie zakonczyc dzialania
}

void main()
{
	clear_s();
	init_descriptor_tables();
	create_task_manager(&task_manager);
	//menu musi być dodane pierwsze
	add_task(system_menu);

	add_task(&floppy_reset);

	//Musi być ostatnie bo uruchamia wielowątkowość
	irq_install();
	while(1);
}

