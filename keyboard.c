#include "keyboard.h"
#include "ports.h"
#include "isr.h"
#include "screen.h"
#include "input.h"

#define ENTER 0x1c
#define BACKSPACE 14

char key_buf[256];

//znaki ASCII wg. odpowiadajacych im scancodow
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     			
    	'7', '8', '9', '0', '-', '=', 'x', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static u32int keyboard_callback(u32int esp)
{
	u8int scancode = port_byte_in(0x60);

	if (scancode > 57) return esp;

	if(scancode == ENTER)
		special_key(1);
	else if(scancode == BACKSPACE)
		special_key(2);
	else
	{	
		char l = sc_ascii[(int)scancode];
		add_char(l);
	}
    return esp;
}

void init_keyboard() 
{
   register_interrupt_handler(IRQ1, &keyboard_callback); 

}