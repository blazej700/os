#ifndef PROCESS_H
#define PROCESS_H

#include "../utils/types.h"
#include "isr.h"

typedef struct 
{
    u8int stack[4096];
    CPUState *regs;
    process_status status;
    unsigned int timer_ticks;
    void (*real_entrypoint)();
    char name[];
} Task;

typedef struct
{
    Task tasks[256];
    int num_tasks;
    int current_task;
} TaskManager;

void create_task(Task *newTask, void entrypoint(), char name[]);
void create_task_manager(TaskManager *newTaskManager);
bool add_task(void entrypoint(), char name[]);
u32int schedule(CPUState *regs);
void kill_task(int task_id);
void process_wrapper();
void list_processes();
extern void yield();

TaskManager task_manager;

#endif