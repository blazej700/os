#include "kernel.h"

//Sprawdzamy czy na pewno nie kompilujemy dla os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
 
//Czy na pewno kompilujemy w 32bit
#if !defined(__i386__)
#error "This needs to be compiled with a i686-elf compiler"
#endif

char param[256];

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

	u32int size;
	size = (u32int)(str_to_int(param));

	u32int phys_addr;
	u32int page = kmalloc(size, &phys_addr);

	print_f("Page: %x, physical address: %p\n", page, phys_addr);
}

void edit()
{

	char values[2000];
	// Paskudny hack, zeby nie wracalo do menu
	task_manager.tasks[0].status = DEAD_PROCESS;

	print("Text: \n");
	scan_c(values);

	task_manager.tasks[0].status = ACTIVE_PROCESS;

	write_to_file(param,values);
}
void dir(){

	print_files();
}

void view_c(){

	dump_file(param, false);
}

void view_h(){

	dump_file(param, true);
}

void help()
{

	print_f("Available commands:\n");
	print_f("%-10s %-10s %s\n\n","Command", "Params", "Description");
	print_f("%-10s %-10s %s\n","STOP","","Stoping processor.");
	print_f("%-10s %-10s %s\n","DIR","","Listing files.");
	print_f("%-10s %-10s %s\n","TIME","","Display time.");
	print_f("%-10s %-10s %s\n","SETTIME","","Set time.");
	print_f("%-10s %-10s %s\n","PS","","Listing processes.");
	print_f("%-10s %-10s %s\n","KILL","number","Killing process.");
	print_f("%-10s %-10s %s\n","SCHEDULER","","Scheduler testing.");
	print_f("%-10s %-10s %s\n","MALLOC","size","Malloc testing.");

	print_f("%-10s %-10s %s\n","HEX","File_name","Display file in hex.");
	print_f("%-10s %-10s %s\n","VIEW","File_name","Display file.");
	print_f("%-10s %-10s %s\n","EX","File_name","Executing file.");
	print_f("%-10s %-10s %s\n","EDIT","File_name","Editing file.");
	print_f("%-10s %-10s %s\n","DRIVES","File_name","Printing drives list.");

	print_f("%-10s %-10s %s\n","A/B","","Change floppy drive to A or B.");


}
void ex(){

	open_program(param);

}
void drives_print(){
	print_f("Available floppy drives:\n");
	print_f("Drive capacity[kB] size[\"]\n");
	floppy_detect_drives();
	
}

void heloo_msg(){
	print_f(" ------------------------------------------------- \n");
	print_f("|%-49s |\n","                      _      __  ___      ");
	print_f("|%-49s |\n","                     | |    /_ |/ _ \\ ");
	print_f("|%-49s |\n","  _ __ ___   ___   __| |_   _| | (_) |");
	print_f("|%-49s |\n"," | '_ ` _ \\ / _ \\ / _` | | | | |> _ < / _ \\/ __|");
	print_f("|%-49s |\n"," | | | | | | (_) | (_| | |_| | | (_) | (_) \\__ \\ ");
	print_f("|%-49s |\n"," |_| |_| |_|\\___/ \\__,_|\\__,_|_|\\___/ \\___/|___/");
	print_f("|%-49s |\n","         ___  __ ");
	print_f("|%-49s |\n","        / _ \\/_ | ");
	print_f("|%-49s |\n"," __   _| | | || | ");
	print_f("|%-49s |\n"," \\ \\ / | | | || |");
	print_f("|%-49s |\n","  \\ V /| |_| _| | ");
	print_f("|%-49s |\n","   \\_/  \\___(_|_| ");
	print_f(" ------------------------------------------------- \n");
}



void system_menu()
{

	heloo_msg();
	print("\n");
	drives_print();

	char drive_l='A';

	char input[256];

	while(1)
	{
		print_f("%c>", drive_l);
		int a;
		a = scan_f("%s %s",input, param);

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
			add_task(scheduler_test,"scheduler");
			print("\n");
	    }
	    else if(strcmp(input, "PS") == 1)
	    {
	    	list_processes();
	    	print("\n");
	    }
	    else if(strcmp(input, "KILL") == 1)
	    {
			int id;
			id = str_to_int(param);
			kill_task(id);
			print("\n");
	    }
	    else if(strcmp(input, "EDIT") == 1)
	    {
			add_task(edit,"edit");
			print("\n");
	    }
	    else if(strcmp(input, "DIR") == 1)
	    {
			add_task(dir,"dir");
			print("\n");

	    }
	    else if(strcmp(input, "HELP") == 1)
	    {
			help();
			print("\n");
	    }
	    else if(strcmp(input, "VIEW") == 1)
	    {
			add_task(view_c,"view_c");
			print("\n");

	    }
	    else if(strcmp(input, "HEX") == 1)
	    {
			add_task(view_h,"view_h");
			
	    }
	    else if(strcmp(input, "DRIVES") == 1)
	    {
			drives_print();
			
	    }
	    else if(strcmp(input, "EX") == 1)
	    {
			add_task(ex,"ex");
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
		    //print_f("%s\n", input);
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
	add_task(system_menu, "main");

	add_task(&floppy_reset,"floppy_reset");

	//Musi być ostatnie bo uruchamia wielowątkowość
	irq_install();
	while(1);
}

