#include "input.h"
#include "types.h"
#include "util.h"

char buf[256];
int special;
u8int scan_flag = 0;

void add_char(char c)
{
	if(scan_flag == 1)
	{
		char str[2] = {c, '\0'};
		append(buf, c);
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
			int len = strl(buf);

			if(len > 0)
			{
				delend(buf);
				remove_last();
			}
		}
		special = a;
	}
}

void scan_c(char *c)
{
	int i = 0;
	while(c[i])
		c[i++]='\0';
	
	scan_flag = 1;

	while(special!=1 && scan_flag);

	str_copy(c,buf);

	buf[0] = '\0';

	special = 0;
	scan_flag = 0;
}