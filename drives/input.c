#include "input.h"


char input_buf[256];
int special;
u8int scan_flag = 0;

void add_char(char c)
{
	if(scan_flag == 1)
	{
		char str[2] = {c, '\0'};
		append(input_buf, c);
		print(str);
	}
}

void special_key(int a)
{
	if(scan_flag == 1)
	{		
		if(a==1)
		{
			print("\n");
		}
		else if(a==2)
		{
			int len = strl(input_buf);

			if(len > 0)
			{
				delend(input_buf);
				remove_last();
			}
		}
		special = a;
	}
}

void scan_c(char *c)
{
	int i = 0;
	while(c[i]){
		c[i++]='\0';
	}
	
	scan_flag = 1;

	while(special!=1 && scan_flag);

	i=str_copy(c,input_buf);

	for(i=0; i<256; i++)
		input_buf[i] = '\0';


	special = 0;
	scan_flag = 0;
}