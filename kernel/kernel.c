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
	print("\nCurent time (h.m.s): ");
	scan_f("%d.%d.%d", &init_time.h, &init_time.m, &init_time.s);
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

void edit()
{
	int i=0;
	char file_name[10];
	char values[2000];
	// Paskudny hack, zeby nie wracalo do menu
	task_manager.tasks[0].status = DEAD_PROCESS;
	print("File name: ");
	scan_c(file_name);


	print("Text: \n");
	scan_c(values);

	task_manager.tasks[0].status = ACTIVE_PROCESS;

	write_to_file(file_name,values);
}
void dir(){

	print_files();
	print("\n");
}

void view_c(){
	char name[9];

	task_manager.tasks[0].status = DEAD_PROCESS;
	print("FIle name:");
	scan_c(name);
	print("\n");

	task_manager.tasks[0].status = ACTIVE_PROCESS;

	dump_file(name, false);
	print("\n");
}

void view_h(){
	char name[9];

	task_manager.tasks[0].status = DEAD_PROCESS;
	print("FIle name:");
	scan_c(name);
	print("\n");
	

	task_manager.tasks[0].status = ACTIVE_PROCESS;

	dump_file(name, true);
	print("\n");
}

void help()
{

	print("To stop, type STOP.\n");
	print("To set  time, type SETTIME.\n");
	print("To curent time, type TIME.\n");
	print("To kmalloc, type MALLOC.\n"); 
	print("To test scheduler, type SCHEDULER.\n"); 
	print("To list processes, type PS.\n"); 
	print("To kill process, type KILL.\n"); 
	print("To test floppy, type FLOPPY.\n"); 
	print("DIR to list files");
}
void ex(){
	char name[9];

	task_manager.tasks[0].status = DEAD_PROCESS;
	print("FIle name:");
	scan_c(name);
	

	task_manager.tasks[0].status = ACTIVE_PROCESS;

	open_program(name);
	print("\n");
}



void system_menu()
{
	print_r("OS v1\n");
	print_r("Dzien dobry\n");

	char drive_l='A';

	char input[256];
	while(1)
	{
		print_f("%c>", drive_l);
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
	    	print("\n");
	    }
	    else if(strcmp(input, "MALLOC") == 1)
	    {
	    	malloc_test();
	    	print("\n");
	    }
	    else if(strcmp(input, "SETTIME") == 1)
	    {
	    	clock_init();
	    	print("\n");
	    }
	    else if(strcmp(input, "SCHEDULER") == 1)
	    {
			add_task(scheduler_test);
			print("\n");
	    }
	    else if(strcmp(input, "PS") == 1)
	    {
	    	list_processes();
	    	print("\n");
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
			print("\n");
	    }
	    else if(strcmp(input, "EDIT") == 1)
	    {
			add_task(edit);
			print("\n");
	    }
	    else if(strcmp(input, "DIR") == 1)
	    {
			add_task(dir);
			print("\n");

	    }
	    else if(strcmp(input, "HELP") == 1)
	    {
			help();
			print("\n");
	    }
	    else if(strcmp(input, "VIEW") == 1)
	    {
			add_task(view_c);
			print("\n");

	    }
	    else if(strcmp(input, "HEX") == 1)
	    {
			add_task(view_h);
			print("\n");

	    }
	    else if(strcmp(input, "EX") == 1)
	    {
			add_task(ex);
			print("\n");

	    }
	    else if(strcmp(input, "") == 1)
	    {
	    	continue;
	    }
	    else if(strcmp(input, "A") == 1)
	    {
	    	if(change_drive(0))
	    		drive_l='A';
	    }
	    else if(strcmp(input, "B") == 1)
	    {
	    	if(change_drive(1))
	    		drive_l='B';
	    }
	    else
		{	    
		    print_f("%s, command not found. To help, type HELP\n", input);
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

