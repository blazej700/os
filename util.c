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
void str_copy(char a[], char b[]) {
    int i = 0;

    while(b[i]){
        a[i]=b[i];
        i++;
    }
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
    int l = strl(s);
    int j = 0;
    l--;
    for(; l>=0; l--){
        i = i + (s[l] - '0')*pow(10,j);
        j++;
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