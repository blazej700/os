

void print_g()
{
	char napis[]={'n','i','e'};
	char* video_memory = (char*) 0xb8000;
	for(int i; i<15; i++){
		*video_memory = napis[i];
		video_memory=video_memory+2;
	}
}

void main()
{
	print_g();
}
