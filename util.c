#include "util.h"


/*  Przydatne funkcje
 *
 *
 */


void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
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

void backspace(char s[]) {
    int len = strl(s);
    s[len-1] = '\0';
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