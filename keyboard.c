#include "keyboard.h"
#include "ports.h"
#include "isr.h"
#include "screen.h"

#define ENTER 0x1c

char key_buf[256];

//znaki ASCII wg. odpowiadajacych im scancodow
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     			
    	'7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static void keyboard_callback(registers_t r)
{

	u8int scancode = port_byte_in(0x60);

	if (scancode > 57) return;

	if(scancode == ENTER)
	{
		print("\n");
		user_input(key_buf);
		key_buf[0] = '\0';
	}
	else
	{	
		
		char l = sc_ascii[(int)scancode];

		char str[2] = {l, '\0'};
		append(key_buf, l);
		print(str);
	}
	
}

void init_keyboard() 
{
   register_interrupt_handler(IRQ1, &keyboard_callback); 

}