#include "screen.h"

/*  wyswietla msg od podanego miejsca
/   *msg - wskaznik na wiadomosc do wyswietlenia 
/   col - kolumna
/   row - wiersz
*/
void print_at(char *msg, int col, int row, char color)
{
    int offset;
    int i;
    int size;

    offset=get_offset(col, row);
    size=strl(msg);

    for(i=0; i < size; i++)
    {
        offset = print_c(msg[i], col, row, color);
        col = get_offset_col(offset);
        row = get_offset_row(offset);
    }
}

 /* Czysci ostatni znak i przesuwa kursor na wczesniejsza pozycje
 /
*/ 
void remove_last()
{
    int offset;
    int row;
    int col;
    char color;

    color=DEFAULT_COLOR;
    offset = get_cursor();
    offset = offset - 1;
    row = get_offset_row(offset);
    col = get_offset_col(offset);
    
    print_c(' ', col, row, color);

    set_cursor(offset);
}

/*  wyswietla msg od miejsca gdzie znajduje sie kursor
/   *msg - wskaznik na wiadomosc do wyswietlenia
*/
void print(char *msg)   
{
    int offset;
    int row;
    int col;
    char color;

    color=DEFAULT_COLOR;
    offset = get_cursor();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
    
    print_at(msg, col, row, color);
}

void print_r(char *msg)
{
    int offset;
    int row;
    int col;
    char color;
    int size;
    int i;

    color=0x01;
    offset = get_cursor();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
    
    offset=get_offset(col, row);
    size=strl(msg);
    for(i=0; i < size; i++)
    {
        offset = print_c(msg[i], col, row, color);
        col = get_offset_col(offset);
        row = get_offset_row(offset);
        if(color>0x0e)
            color=0x01;
        else
            color++;
    }
}


/*  wyswietla znak w zadanym miejscu
/   a - znak do wyswietlenia
/   col - kolumna
/   row - wiersz 
/   color - kolor liter i tla 
/   zwraca offset - przesuniecie 
*/  
int print_c(char a, int col, int row, char color)   
{
    char *screen = (char *)VIDEO_ADDRESS;

    int offset;
    offset=get_offset(col, row);

    if(a=='\n') //jak nasz znak to znak nowej lini to ustawiamy kolumne na 0 i wiersz na kolejny
    {
        offset=get_offset(0, row+1);
        set_cursor(offset);
        return offset; 
    }
    else    //zapisujemy nasz znak i kolor w pamieci 
    {
        screen[offset]=a;
        screen[offset+1]=color;
    }


    if((col+1)< MAX_COLS)               //przesuwamy na kolejna kolumne
        offset=get_offset(col+1, row);
    else                                //albo na poczatek nowej lini jesli nie ma juz miejsca
        offset=get_offset(0, row+1); 

    if((row+3) > MAX_ROWS)      //jesli zaraz bedzie koniec to wszystko przesuwamy do gory
        offset=scroll(offset);

    set_cursor(offset);  //ustawiamy kursor na nowa pozycje
    return offset;  
}

/* Czysci wyswietlacz 
 *
 */
void clear_s() 
{
    int size = MAX_ROWS*MAX_COLS;   //chyba oczywiste xd
    char *screen = (char *)VIDEO_ADDRESS;   //wzkaznik na pamiec vga
    int i;
    for(i=0; i<size; i++)           //wedrujemy sobie przez cala pamiec i 
    {
        screen[i*2] = ' ';          //ustawiamy znak na ' '
        screen[i*2+1] = DEFAULT_COLOR; // i kolor na domyslny
    }
    set_cursor(0);
}

/* Przesuwa wszytko do gory
 *
 */
int scroll(int offset)
{
    
    int i;
    for (i = 1; i < MAX_ROWS; i++) 
    {
        memory_copy((char *)(get_offset(0, i) + VIDEO_ADDRESS), (char*)(get_offset(0, i-1) + VIDEO_ADDRESS), MAX_COLS * 2);
    }
    /* Blank last line */
    char *last_line = (char *)(get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS);
    for (i = 0; i < MAX_COLS * 2; i++) 
    {
        last_line[i] = 0;
    }
    offset -= 2 * MAX_COLS;
    return offset;

}

/*  zwraca pozycje kursora jako:
 *  offset - przesuniecie 
 */
int get_cursor()    
{

    /* obecna pozycja kursora jest zapisana w portach vga
     * high byte of the cursor offset (data 14)
     * low byte (data 15)
    */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; 
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; 
}

/*  ustawia kursor w zdanym miejscu
/   offset
*/
void set_cursor(int offset)   
{   
    /* znowu trzeba grzebac w pamieci xd
     *
     */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

/* zwraca offset - przesuniecie od poczatku pamieci grafiki w zaleznosci od:
/   col - kolumny
/   row - wiersza
*/
int get_offset(int col, int row) 
{ 
    return 2 * (row * MAX_COLS + col); 
}

/*  zwraca row - wiersz w zaleznosci od
/   offset - przesuniecia 
*/
int get_offset_row(int offset) 
{ 
    return offset / (2 * MAX_COLS); 
}

/*  zwraca 
/   col - kolumne 
/   w zaleznosci od
/   offset - przesuniecia
*/
int get_offset_col(int offset) 
{ 
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}
