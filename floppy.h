#ifndef FLOPPY_H
#define FLOPPY_H

#include "timer.h"
#include "util.h"
#include "isr.h"
// standard base address of the primary floppy controller
static const int floppy_base = 0x03f0;
// standard IRQ number for floppy controllers
static const int floppy_irq = 38;

#define floppy_dmalen_max 0x4800
char floppy_dmabuf[floppy_dmalen_max] __attribute__((aligned(0x8000)));
static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = 0;

// The registers of interest. There are more, but we only use these here.
enum floppy_registers 
{
   FLOPPY_DOR  = 2,  // digital output register
   FLOPPY_MSR  = 4,  // master status register, read only
   FLOPPY_FIFO = 5,  // data FIFO, in DMA operation for commands
   FLOPPY_CCR  = 7   // configuration control register, write only
};

typedef enum 
{
    floppy_dir_read = 1,
    floppy_dir_write = 2
} floppy_dir;

// The commands of interest. There are more, but we only use these here.
enum floppy_commands 
{
   CMD_SPECIFY = 3,            // SPECIFY
   CMD_WRITE_DATA = 5,         // WRITE DATA
   CMD_READ_DATA = 6,          // READ DATA
   CMD_RECALIBRATE = 7,        // RECALIBRATE
   CMD_SENSE_INTERRUPT = 8,    // SENSE INTERRUPT
   CMD_SEEK = 15,              // SEEK
};

static const char * drive_types[8] = 
{
    "none",
    "360kB 5.25\"",
    "1.2MB 5.25\"",
    "720kB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "unknown type",
    "unknown type"
};

enum 
{ 
    floppy_motor_off = 0, 
    floppy_motor_on, 
    floppy_motor_wait 
};

void floppy_detect_drives();
void floppy_write_cmd(char cmd);
unsigned char floppy_read_data();
void floppy_check_interrupt(int *st0, int *cyl);
void floppy_motor(int onoff);
void floppy_motor_kill();
void floppy_timer();
int floppy_calibrate();
int floppy_reset();
int floppy_seek(int cyli);
int floppy_do_track(int cyl, int head, int sector, floppy_dir dir, int size);
int floppy_read_track_chs(int cyl, int head, int sector, int size);
int floppy_write_track_chs(int cyl, int head, int sector, int size);
void lba_2_chs(int lba, int* cyl, int* head, int* sector);
int floppy_read_track(int lba, int size);
int floppy_write_track(int lba, int size);
#endif