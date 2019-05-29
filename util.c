#include "util.h"


/*  Przydatne funkcje
 *
 *
 */

u32int free_mem_addr = 0x10000; //Początek wolnej pamięci, czyli moment od ktorego zapisujemy nasze jadro w pamieci

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_shift(char *source, int len, int offset) {
	char *end = source + len;

	if (offset > 0) {
		for (; end >= source; end--) {
			*(end + offset) = *end;
			*end = '\0';
		}
	} else if (offset < 0) {
		for (; source <= end; source++) {
			*(source + offset) = *source;
			*source = '\0';
		}
	}
}

void swap(char *a1, char *a2){
    char tmp;
    tmp = *a1;
    *a1 = *a2;
    *a2 = tmp;
}

void rev(char str[], int len){
    int j = 0;
    while(j<len/2){
        swap(&str[j], &str[len-j-1]);
        j++;
    }
}


void int_to_ascii(int n, char str[]) {

    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';

    rev(str, i);

    str[i] = '\0';
}

void float_to_ascii(float n, char str[], int prec) {
	int whole, fraction, sign = 0, i = 0;

	if (prec > 6) prec = 6;

	if (n < 0) {
		sign = 1;
		n = -n;
	}

	whole = n;
	fraction = (n - whole) * pow(10, prec);

	if (sign) str[i++] = '-';
	int_to_ascii(whole, &str[i]);
	while (str[i] != '\0') i++;
	str[i++] = '.';
	int_to_ascii(fraction, &str[i]);
}

void str_format_var(char *buf, char *format, va_list args) {
	// simplified format string: %[flag][width][.precision][type]

	// processing mode flag, 1 when processing format specifier
	int proc = 0;

	for (; *format; format++) {
		int width = 0, prec = 0, lpad = 0;
		int len = 0, fill = 0;
		char fillc = ' ';
		union arg_type {
			int i; float f; char c; char *s; u32int ui;
		} arg;

		// '%' means start of format specifier
		if (*format == '%') {
			// first '%' starts processing, second ends it
			if ((proc = !proc)) continue;
		}

		// ordinary text
		if (!proc) {
			*buf++ = *format;
			continue;
		}

		// processing specfier
		// flag
		while (*format == '-' || *format == '0') {
			if (*format == '-') lpad = 1;
			if (*format == '0') fillc = '0';
			format++;
		}
		// width
		while (*format >= '0' && *format <= '9') {
			width *= 10;
			width += *format++ - '0';
		}
		// precision
		if (*format == '.') {
			format++;
			while (*format >= '0' && *format <= '9') {
				prec *= 10;
				prec += *format++ - '0';
			}
		}
		// type
		switch (*format) {
			case 'i':
			case 'd':
				arg.i = va_arg(args, int);
				int_to_ascii(arg.i, buf);
				while (*(buf + len)) len++;
				break;
			case 'f':
				arg.f = va_arg(args, double);
				float_to_ascii(arg.f, buf, (prec ? prec : 2));
				while (*(buf + len)) len++;
				break;
			case 'x':
			case 'p':
				arg.ui = va_arg(args, u32int);
				hex_to_str(arg.ui, buf);
				while (*(buf + len)) len++;
				break;
			case 's':
				arg.s = va_arg(args, char*);
				len += str_copy(buf, arg.s);
				break;
			case 'c':
				arg.c = va_arg(args, int);
				*buf = arg.c;
				len++;
				break;
			case 'b':
				arg.i = va_arg(args, int);
				len += str_copy(buf, (arg.i ? "true" : "false"));
				break;
			default:
				break;
		}
		// fill to specified width
		fill = width - len;
		if (fill > 0) {
			if (lpad) {
				memory_set((u8int*) (buf + len), fillc, fill);
			} else {
				memory_shift(buf, len, fill);
				memory_set((u8int*) buf, fillc, fill);
			}
			buf += width;
		} else {
			buf += len;
		}
		proc = 0;
	}

	va_end(args);
	*buf = '\0';
}

void str_format(char *buf, char *format, ...) {
	va_list args;
	va_start(args, format);
	str_format_var(buf, format, args);
	va_end(args);
}

void print_f(char *format, ...) {
	char buf[PRINT_BUF_LEN];

	va_list args;
	va_start(args, format);
	str_format_var(buf, format, args);
	va_end(args);

	print(buf);
}

int str_read_format_var(char *buf, char *format, va_list args) {
	// simplified format string: %[width][type]

	// number of correctly matched items
	int n = 0;
	// processing mode flag, 1 when processing format specifier
	int proc = 0;

	for (; *format; format++) {
		int width = 0;
		int len = 0;
		union out_type {
			int *i; float *f; char *c; u32int ui; u32int* p;
		} out;

		// '%' means start of format specifier
		if (*format == '%') {
			// first '%' starts processing, second ends it
			if ((proc = !proc)) continue;
		}

		// ordinary text
		if (!proc) {
			// continue if the text is matching format
			if (*format == *buf++) continue;
			// return if not
			return n;
		}

		// processing specifier
		// width
		while (*format >= '0' && *format <= '9') {
			width *= 10;
			width += *format++ - '0';
		}
		// type
		switch (*format) {
			case 'i':
			case 'd':
				*(va_arg(args, int*)) = str_to_int(buf);
				while (is_digit(buf[len])) len++;
				break;
			case 'f':
				*(va_arg(args, float*)) = str_to_float(buf);
				while (is_digit(buf[len])) len++;
				if (buf[len] == '.') len++;
				while (is_digit(buf[len])) len++;
				break;
			case 'x':
				*(va_arg(args, u32int*)) = hex_str_to_int(buf);
				while (is_digit(buf[len]) ||
					   (buf[len] >= 'a' && buf[len] <= 'f') ||
					   (buf[len] >= 'A' && buf[len] <= 'F')) len++;
				break;
			case 's':
				out.c = va_arg(args, char*);
				while (!is_whitespace(buf[len])) len++;
				memory_copy(buf, out.c, len);
				out.c[len] = '\0';
				break;
			case 'c':
				*(va_arg(args, char*)) = *buf;
				len++;
				break;
			case 'b':
				out.i = va_arg(args, int*);
				if (strcmp("true", buf)) {
					*(out.i) = 1;
					len += 4;
				} else if(strcmp("false", buf)) {
					*(out.i) = 0;
					len += 5;
				}
				break;
			default:
				break;
		}
		if (!len) return n;
		buf += len;
		n++;
		proc = 0;
	}

	va_end(args);
	*buf = '\0';

	return n;
}

int str_read_format(char *buf, char *format, ...) {
	int n;
	va_list args;

	va_start(args, format);
	n = str_read_format_var(buf, format, args);
	va_end(args);

	return n;
}

int scan_f(char *format, ...) {
	int n;
	char buf[PRINT_BUF_LEN];
	va_list args;

	scan_c(buf);
	va_start(args, format);
	n = str_read_format_var(buf, format, args);
	va_end(args);

	return n;
}

void memory_set(u8int *dest, u8int val, u32int len) {
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) 
    {
        *temp++ = val;
    }
}

int strl(char* s)
{
    int i;
    i=0;
    while(s[i])
    {
        i++;
    }

    return i;
}

void append(char s[], char n) {
    int len = strl(s);
    s[len] = n;
    s[len+1] = '\0';
}

void delend(char s[]) {
    int len = strl(s);
    s[len-1] = '\0';
}

/*
 *  a=b
 */
int str_copy(char a[], char b[]) {
    int i = 0;

    while(b[i]){
        a[i]=b[i];
        i++;
    }

    return i;
}


/* 
 * jesli sie zgadzaja zwraca 1, jesli nie 0
 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) 
    {
        if (s1[i] == '\0') 
            return 1;
    }
    return 0;
}

int pow(int a, int b)
{
    if(b == 0)
        return 1;
    if(a == 0)
        return 0;
    int tmp = a;
    for( ; b>1; b--)
        tmp=tmp*a;
    return tmp;
}

int str_to_int(char s[])
{
	int i = 0;

	while (is_digit(*s)) {
		i *= 10;
		i += *s++ - '0';
	}

	return i;
}

float str_to_float(char s[]) {
	float f = 0;
	int numerator, denominator = 1;

	f = str_to_int(s);
	while (is_digit(*s)) s++;
	if (*s != '.') return f;
	numerator = str_to_int(++s);
	while (is_digit(*s)) {
		s++;
		denominator *= 10;
	}
	f += (float) numerator / denominator;

	return f;
}

u32int hex_str_to_int(char s[]) {
	u32int i = 0;

	char c;
	while ((c = *s++)) {
		if (is_digit(c)) c = c - '0';
		else if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
		else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
		else break;
		i = (i << 4) | (c & 0xF);
	}

	return i;
}

void hex_to_str(u32int hex, char str[])
{
    int i;
    
    i = 0;
    do {
        if(hex % 0x10 < 0xa)
            str[i] = hex % 0x10 + '0';
        else{
            str[i] = hex % 0x10 + 'A' - 0xa;
        }
        i++;
    } while ((hex /= 0x10) > 0);

    str[i++]='x';
    str[i++]='0';

    rev(str, i);

    str[i] = '\0';
}

int is_letter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_digit(char c) {
	return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
	return (c == ' '  ||
			c == '\f' ||
			c == '\n' ||
			c == '\r' ||
			c == '\t' ||
			c == '\v' ||
			c == '\0');
}

 /* zwraca wskaznik na wolna pamiec po zaalokowaniu 
  * size - rozmaiar tego czego chcemy zaalokowac
  * phys_addr - wskaznik na to co chcemy przydzielic, zmieniamy tez jego wartosc na pocztek wolej pamieci
  */
u32int kmalloc(u32int size, u32int *phys_addr) 
{

    // Kazda strona ma 4kb
    if ((free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000; // 0x1000b == 4kb
    }
    

    if (phys_addr){
        *phys_addr = free_mem_addr;
    }

    u32int ret = free_mem_addr;
    free_mem_addr += size; 
    return ret;
}
