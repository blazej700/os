#include "floppy.h"

u8int current_drive = 0;


bool change_drive(int d){
   port_byte_out(0x70, 0x10);
   unsigned drives = port_byte_in(0x71);

   if(d == current_drive || d>1){
      return false;
   }

   if(current_drive==0 && (drives & 0xf)){
      current_drive=1;
      return true;
   }
   else{
      current_drive=0;
      return true;
   }
}

// Mozna uzyc do sprawdzenia jakie dyskietki sa wlozone
void floppy_detect_drives() 
{
   port_byte_out(0x70, 0x10);
   unsigned drives = port_byte_in(0x71);

   print_f(" - Floppy drive 0: %s\n", drive_types[drives >> 4]);
   print_f(" - Floppy drive 1: %s\n", drive_types[drives & 0xf]);
}

//
// The MSR byte: [read-only]
// -------------
//
//  7   6   5    4    3    2    1    0
// MRQ DIO NDMA BUSY ACTD ACTC ACTB ACTA
//
// MRQ is 1 when FIFO is ready (test before read/write)
// DIO tells if controller expects write (1) or read (0)
//
// NDMA tells if controller is in DMA mode (1 = no-DMA, 0 = DMA)
// BUSY tells if controller is executing a command (1=busy)
//
// ACTA, ACTB, ACTC, ACTD tell which drives position/calibrate (1=yes)
//
//
void floppy_write_cmd(char cmd) 
{
   int i; 
   for(i = 0; i < 60; i++) 
   {
      sleep(1);
      if(0x80 & port_byte_in(floppy_base+FLOPPY_MSR)) 
         return (void) port_byte_out(floppy_base+FLOPPY_FIFO, cmd);
   }
    print_f("floppy_write_cmd: timeout\n");    
}

unsigned char floppy_read_data() 
{
   int i; 
   for(i = 0; i < 60; i++) 
   {
      sleep(1); 
      if(0x80 & port_byte_in(floppy_base+FLOPPY_MSR))
         return port_byte_in(floppy_base+FLOPPY_FIFO);
   }
   print_f("floppy_read_data: timeout");
   return 0; 
}

void floppy_check_interrupt(int *st0, int *cyl) 
{
   floppy_write_cmd(CMD_SENSE_INTERRUPT);
   *st0 = floppy_read_data();
   *cyl = floppy_read_data();
}

//
// The DOR byte: [write-only]
// -------------
//
//  7    6    5    4    3   2    1   0
// MOTD MOTC MOTB MOTA DMA NRST DR1 DR0
//
// DR1 and DR0 together select "current drive" = a/00, b/01, c/10, d/11
// MOTA, MOTB, MOTC, MOTD control motors for the four drives (1=on)
//
// DMA line enables (1 = enable) interrupts and DMA
// NRST is "not reset" so controller is enabled when it's 1
//
void floppy_motor(int onoff) 
{
   if(onoff) 
   {
      if(!floppy_motor_state) 
      {
         // trzeba uruchomic
         if(current_drive == 0)
            port_byte_out(floppy_base + FLOPPY_DOR, 0x1c);
         if(current_drive == 1)
            port_byte_out(floppy_base + FLOPPY_DOR, 0x2D);

         sleep(50); 
      }
      floppy_motor_state = floppy_motor_on;
   } else 
   {
      if(floppy_motor_state == floppy_motor_wait) 
         print("floppy_motor: strange, fd motor-state already waiting..\n");
      floppy_motor_ticks = 300; //floppy_timer
      floppy_motor_state = floppy_motor_wait;
   }
}

void floppy_motor_kill() 
{

   if(current_drive == 0)
         port_byte_out(floppy_base + FLOPPY_DOR, 0x0c);
   if(current_drive == 1)
         port_byte_out(floppy_base + FLOPPY_DOR, 0x0D);

   floppy_motor_state = floppy_motor_off;
}

void floppy_timer() 
{
   while(1) 
   {
      sleep(50);
      if(floppy_motor_state == floppy_motor_wait) 
      {
         floppy_motor_ticks -= 50;
         if(floppy_motor_ticks <= 0)
            floppy_motor_kill();
      }
   }
}

// Przesun do cylindra 0
int floppy_calibrate() 
{
   int i, st0, cyl = -1;
   floppy_motor(floppy_motor_on);

   for(i = 0; i < 10; i++) 
   {
      floppy_write_cmd(CMD_RECALIBRATE);
      //floppy_write_cmd(0); // ktory dysk
      floppy_write_cmd(current_drive);
        
      irq_wait(floppy_irq);
      floppy_check_interrupt(&st0, &cyl);
      
      if(st0 & 0xC0) 
      {
         static const char * status[] = { 0, "error", "invalid", "drive" };
         print_f("floppy_calibrate: status = %s\n", status[st0 >> 6]);
         continue;
      }

      if(!cyl) 
      { 
         // na cylindrze 0
         floppy_motor(floppy_motor_off);
         return 0;
      }
   }

   print("floppy_calibrate: 10 retries exhausted\n");
   floppy_motor(floppy_motor_off);
   return -1;
}

int floppy_reset() 
{
   port_byte_out(floppy_base + FLOPPY_DOR, 0x00); // wylacz
   if(current_drive==0)
      port_byte_out(floppy_base + FLOPPY_DOR, 0x0C); // wlacz
   if(current_drive==1)
      port_byte_out(floppy_base + FLOPPY_DOR, 0x0D);

   irq_wait(floppy_irq);

   int st0, cyl;
   floppy_check_interrupt(&st0, &cyl);

   // set transfer speed 500kb/s
   port_byte_out(floppy_base + FLOPPY_CCR, 0x00);

   //  - 1st byte is: bits[7:4] = steprate, bits[3:0] = head unload time
   //  - 2nd byte is: bits[7:1] = head load time, bit[0] = no-DMA
   // 
   //  steprate    = (8.0ms - entry*0.5ms)*(1MB/s / xfer_rate)
   //  head_unload = 8ms * entry * (1MB/s / xfer_rate), where entry 0 -> 16
   //  head_load   = 1ms * entry * (1MB/s / xfer_rate), where entry 0 -> 128
   //
   floppy_write_cmd(CMD_SPECIFY);
   floppy_write_cmd(0xdf); /* steprate = 3ms, unload time = 240ms */
   floppy_write_cmd(0x02); /* load time = 16ms, no-DMA = 0 */

   if(floppy_calibrate()) 
      return -1;
   return 0;
}



//p rzesun do cylindra
int floppy_seek(int cyli) 
{
   int i, st0, cyl = -1, head=0;
   floppy_motor(floppy_motor_on);

   for(i = 0; i < 10; i++) 
   {
      // 1st byte bit[1:0] = drive, bit[2] = head
      // 2nd byte is cylinder number
      floppy_write_cmd(CMD_SEEK);
      floppy_write_cmd(head<<2|current_drive);
      //floppy_write_cmd(head<<2);
      floppy_write_cmd(cyli);
      irq_wait(floppy_irq);
      floppy_check_interrupt(&st0, &cyl);

      if(st0 & 0xC0) 
      {
         static const char * status[] = { "normal", "error", "invalid", "drive" };
         print_f("floppy_seek: status = %s\n", status[st0 >> 6]);
         continue;
      }

      if(cyl == cyli) 
      {
         floppy_motor(floppy_motor_off);
         return 0;
      }
   }
   print_f("%x floppy_seek: 10 retries exhausted\n", floppy_base);
   floppy_motor(floppy_motor_off);
   return -1;
}

static void floppy_dma_init(floppy_dir dir, int floppy_dmalen) 
{
   u32int a;
   u32int c;
   a = (unsigned) &floppy_dmabuf;
   c = (unsigned) floppy_dmalen - 1; // -1 bo DMA liczy dziwnie
   // check that address is at most 24-bits (under 16MB)
   // check that count is at most 16-bits (DMA limit)
   // check that if we add count and address we don't get a carry
   // (DMA can't deal with such a carry, this is the 64k boundary limit)
   if((a >> 24) || (c >> 16) || (((a&0xffff)+c)>>16)) 
        print("floppy_dma_init: static buffer problem\n");

   unsigned char mode;
   switch(dir) 
   {
      // 01:0:0:01:10 = single/inc/no-auto/to-mem/chan2
      case floppy_dir_read:  
         mode = 0x46; break;
      // 01:0:0:10:10 = single/inc/no-auto/from-mem/chan2
      case floppy_dir_write: 
         mode = 0x4a; break;
      default: 
         print("floppy_dma_init: invalid direction");
   }

   port_byte_out(0x0a, 0x06); 
   port_byte_out(0x0c, 0);

   port_byte_out(0x0b, mode);

   port_byte_out(0x04, a & 0xff); //  - address low byte
   port_byte_out(0x04, (a>>8)&0xff); //  - address high byte
   port_byte_out(0x81, (a>>16)&0xff); // external page register

   port_byte_out(0x05, c & 0xff); //  - count low byte
   port_byte_out(0x05, (c>>8)&0xff); //  - count high byte

   port_byte_out(0x0a, 0x02); 
}

int floppy_do_track(int cyl, int head, int sector, floppy_dir dir, int size) 
{
   //print_f("Cylinder %d, head%d, sector%d", cyl, head, sector);
   //head musi byc rowne 0 lub 1
   if(head != 0 && head != 1)
      return 3;

   //sector musi byc wiekszy od zera i mniejszy od 19
   if(sector < 1 || sector > 18) 
      return 4;

   unsigned char cmd;

   // Read is MT:MF:SK:0:0:1:1:0, write MT:MF:0:0:1:0:1
   // where MT = multitrack, MF = MFM mode, SK = skip deleted
   // 
   // Specify multitrack and MFM mode
   static const int flags = 0xC0;
   switch(dir) 
   {
      case floppy_dir_read:
         cmd = CMD_READ_DATA | flags;
         break;
      case floppy_dir_write:
         cmd = CMD_WRITE_DATA | flags;
         break;
      default: 
         print("floppy_do_track: invalid direction");
   }

   if(floppy_seek(cyl)) return -1;

   int i;
   for(i = 0; i < 20; i++) {
      floppy_motor(floppy_motor_on);

      // ustaw DMA
      floppy_dma_init(dir, size);
      sleep(10); // give some time (100ms) to settle after the seeks

      floppy_write_cmd(cmd);  // set above for current direction
      floppy_write_cmd((head<<2)+current_drive);    //0:0:0:0:0:HD:US1:US0 = head and drive
      //floppy_write_cmd(head<<2);
      floppy_write_cmd(cyl);  // cylinder
      floppy_write_cmd(head);    // first head (should match with above)
      floppy_write_cmd(sector);    // first sector, strangely counts from 1
      floppy_write_cmd(2);    // bytes/sector, 128*2^x (x=2 -> 512)
      floppy_write_cmd(18);   // number of tracks to operate on
      floppy_write_cmd(0x1b); // GAP3 length, 27 is default for 3.5"
      floppy_write_cmd(0xff); // data length (0xff if B/S != 0)
        
      irq_wait(floppy_irq);

      unsigned char st0, st1, st2, rcy, rhe, rse, bps;
      st0 = floppy_read_data();
      st1 = floppy_read_data();
      st2 = floppy_read_data();

      rcy = floppy_read_data();
      rhe = floppy_read_data();
      rse = floppy_read_data();
      // bajty na sektor - musi byc to samo co ustawilismy
      bps = floppy_read_data();

      int error = 0;

      if(st0 & 0xC0) 
      {
         static const char * status[] = { 0, "error", "invalid command", "drive not ready" };
         print_f("floppy_do_sector: status = %\n", st0 >> 6);
         error = 1;
      }
      if(st1 & 0x80) 
      {
         print_f("floppy_do_sector: end of cylinder\n");
         error = 1;
      }
      if(st0 & 0x08) 
      {
         print_f("floppy_do_sector: drive not ready\n");
         error = 1;
      }
      if(st1 & 0x20) 
      {
         print_f("floppy_do_sector: CRC error\n");
         error = 1;
      }
      if(st1 & 0x10) 
      {
         print_f("floppy_do_sector: controller timeout\n");
         error = 1;
      }
      if(st1 & 0x04) 
      {
         print_f("floppy_do_sector: no data found\n");
         error = 1;
      }
      if((st1|st2) & 0x01) 
      {
         print_f("floppy_do_sector: no address mark found\n");
         error = 1;
      }
      if(st2 & 0x40) 
      {
         print_f("floppy_do_sector: deleted address mark\n");
         error = 1;
      }
      if(st2 & 0x20) 
      {
         print_f("floppy_do_sector: CRC error in data\n");
         error = 1;
      }
      if(st2 & 0x10) 
      {
         print_f("floppy_do_sector: wrong cylinder\n");
         error = 1;
      }
      if(st2 & 0x04) 
      {
         print_f("floppy_do_sector: uPD765 sector not found\n");
         error = 1;
      }
      if(st2 & 0x02) 
      {
         print_f("floppy_do_sector: bad cylinder\n");
         error = 1;
      }
      if(bps != 0x2) 
      {
         print_f("floppy_do_sector: wanted 512B/sector, got %d", (1<<(bps+7)));
         error = 1;
      }
      if(st1 & 0x02) 
      {
         print_f("floppy_do_sector: not writable\n");
         error = 2;
      }

      if(!error) 
      {
         floppy_motor(floppy_motor_off);
         return 0;
      }
      if(error > 1) 
      {
         print_f("floppy_do_sector: not retrying..\n");
         floppy_motor(floppy_motor_off);
         return -2;
      }
   }

   print_f("floppy_do_sector: 20 retries exhausted\n");
   floppy_motor(floppy_motor_off);
   return -1;
}


int floppy_read_track_chs(int cyl, int head, int sector, int size) 
{
   return floppy_do_track(cyl, head, sector, floppy_dir_read, size);
}

int floppy_write_track_chs(int cyl, int head, int sector, int size) 
{
   return floppy_do_track(cyl, head, sector, floppy_dir_write, size);
}

void lba_2_chs(int lba, int* cyl, int* head, int* sector)
{
    *cyl    = lba / (2 * 18);
    *head   = ((lba % (2 * 18)) / 18);
    *sector = ((lba % (2 * 18)) % 18 + 1);
}

int floppy_read_track(int lba, int size) 
{
   int cyl, head, sector;
   lba_2_chs(lba, &cyl, &head, &sector);
   return floppy_do_track(cyl, head, sector, floppy_dir_read, size);
}

int floppy_write_track(int lba, int size) 
{
   int cyl, head, sector;
   lba_2_chs(lba, &cyl, &head, &sector);
   return floppy_do_track(cyl, head, sector, floppy_dir_write, size);
}