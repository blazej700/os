#include "fat12.h"

fat_file Files[100];
int files_numer = 0;

void load_files_list()
{
	int i=0;

	u32int start;
	u32int size;

	bool flag = true;
	start = first_fat_sector;
	size = max_fats*fat_size;
	floppy_read_track(start, size);


	while(flag){



		if(floppy_dmabuf[i*fat_size]==0x00){
			flag = false;
			break;
		}



		for(int j=0; j<9; j++){
			if(floppy_dmabuf[i*fat_size+j] != 0x20){
				Files[i].name[j]=floppy_dmabuf[i*fat_size+j];
			}
			else if(j==8){
				Files[i].name[j]='\0';
			}
			else{
				Files[i].name[j]='\0';
				break;
			}
		}

		for(int j=0; j<3; j++){
			Files[i].ext[j]=floppy_dmabuf[i*fat_size+ext_offset+j];
		}
		Files[i].ext[4]='\0';

		char c_cluster[2];
		for(int j=0; j<2; j++){
			c_cluster[j]=floppy_dmabuf[cluster_inFat_offset+i*fat_size+j];
		}

		Files[i].cluster=0;
		for(int j=0; j<2; j++){
			Files[i].cluster+=(int)(c_cluster[1-j])*pow(0x10,2-j*2);

		}


		unsigned char c_size[3];
		for(int j=0; j<3; j++){
			c_size[j]=floppy_dmabuf[size_offset+i*fat_size+j];
		}


		Files[i].size=0;
		for(int j=0; j<3; j++){
			Files[i].size+=((int)(c_size[2-j]))*pow(0x10,4-j*2);
		}

		i++;
		files_numer=i;
 	}

	
}



void print_files()
{
	load_files_list();
	int i=0;
	print_f("List of files:\nFile     ext size[b] cluster\n-----------------------------\n");
	for(int i=0; i<files_numer; i++){

		print_f("%-8s %-3s %8d %x \n", Files[i].name, Files[i].ext, Files[i].size, Files[i].cluster, i );
	}
	print_f("Numer of files: %d", files_numer);


}

void dump_file(char name_1[], bool hex)
{

	int i;
	int flag=0;

	load_files_list();

	for(i=0; i<files_numer; i++)
	{	
		flag=strcmp(name_1,Files[i].name);
		if(flag)
			break;
	}

	if(!flag){
		print_f("file %s dont found\n", name_1);
		return;
	}

	floppy_read_track(first_data_sector-2+Files[i].cluster, Files[i].size);
	print_f("File %s.%s:\n",Files[i].name,Files[i].ext);

	if(hex){
		for(int j=0; j<Files[i].size && j<2000; j++)
			print_f("%x: %x\n",&floppy_dmabuf[j], floppy_dmabuf[j]);
	}
	else{
		for(int j=0; j<Files[i].size && j<2000; j++)
			print_f("%c",floppy_dmabuf[j]);
	}

}


void open_program(char name_1[])
{
	int i;
	int flag=0;

	load_files_list();

	for(i=0; i<files_numer; i++)
	{	
		flag=strcmp(name_1,Files[i].name);
		if(flag)
			break;
	}

	if(!flag){
		print_f("file %s dont found\n", name_1);
		return;
	}

	if(!strcmp("BIN",Files[i].ext)){
		print_f("file %s is not exetuble\n", name_1);
		return;
	}

	floppy_read_track(first_data_sector-2+Files[i].cluster, Files[i].size);


	add_task(&floppy_dmabuf[0],name_1);
}

void write_to_file(char name_1[], char values[])
{

	int i;
	int flag=0;

	load_files_list();

	for(i=0; i<files_numer; i++)
	{	
		flag=strcmp(name_1,Files[i].name);
		if(flag)
			break;
	}

	if(!flag){
		print_f("file %s dont found\n", name_1);
		return;
	}

	int j=0;
	for(j=0; j<Files[j].size; j++)
		floppy_dmabuf[j]=0x00;

	floppy_write_track(first_data_sector-2+Files[i].cluster, j);

	for(j=0; values[j]; j++)
		floppy_dmabuf[j]=values[j];


	floppy_write_track(first_data_sector-2+Files[i].cluster, j);

	floppy_read_track(first_fat_sector, max_fats*fat_size);

	
	floppy_dmabuf[size_offset+i*fat_size+0]=j&0x0000ff;
	floppy_dmabuf[size_offset+i*fat_size+1]=(j&0x00ff00)>>0x8;
	floppy_dmabuf[size_offset+i*fat_size+2]=(j&0xff0000)>>0x10;


	floppy_write_track(first_fat_sector, max_fats*fat_size);
}

