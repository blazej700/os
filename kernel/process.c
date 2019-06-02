#include "process.h"

void create_task(Task *newTask, void entrypoint(), char name[])
{
    newTask->regs = (CPUState*)(newTask->stack + 4096 - sizeof(CPUState));

    newTask->regs->eax = 0;
    newTask->regs->ebx = 0;
    newTask->regs->ecx = 0;
    newTask->regs->edx = 0;

    newTask->regs->esi = 0;
    newTask->regs->edi = 0;
    newTask->regs->ebp = (u32int)newTask->regs;

    newTask->regs->eip = (u32int)process_wrapper;

    newTask->regs->cs = 0x8;
    newTask->regs->eflags = 0x202;
    newTask->regs->esp = (u32int)(newTask->regs-4);
    newTask->regs->ss = 0x10;
    newTask->real_entrypoint = (u32int)entrypoint;
    str_copy(newTask->name, name);
    newTask->status = ACTIVE_PROCESS;
}

void create_task_manager(TaskManager *newTaskManager)
{   
    newTaskManager->num_tasks = 0;
    newTaskManager->current_task = -1;
    int i;
    for(i=0; i<143; i++)
        newTaskManager->tasks[i].status = DEAD_PROCESS;
}

bool add_task(void entrypoint(),char name[])
{
    if(task_manager.num_tasks >= 143) 
        return false;

    int i;
    for(i=0; i<143; i++)
    {
        if(task_manager.tasks[i].status == DEAD_PROCESS)
            break;
    }
    create_task(&task_manager.tasks[i], entrypoint, name);
    task_manager.num_tasks++;
    return true;
}

u32int __attribute__((aligned(16))) schedule(CPUState *regs)
{
    if(task_manager.num_tasks <= 0)
        return (u32int)regs;

    if(task_manager.current_task >= 0)
        task_manager.tasks[task_manager.current_task].regs = regs;

    do
    {
        task_manager.current_task++;
        task_manager.current_task %= 143;
    } while(task_manager.tasks[task_manager.current_task].status != ACTIVE_PROCESS);
    return (u32int)task_manager.tasks[task_manager.current_task].regs;
}

void kill_task(int task_id)
{
    if(task_id == 0)
    {
        print("Can't kill main process!\n");
        return;
    }
    if(task_manager.tasks[task_id].status == DEAD_PROCESS)
    {
        print("There is no such process\n");
        return;
    }
    if(task_manager.tasks[task_id].status != SLEEPING_PROCESS)
        task_manager.num_tasks--;

    task_manager.tasks[task_id].status = DEAD_PROCESS;

    
    if(task_id == task_manager.current_task)
        while(1);
}

void process_wrapper()
{
    task_manager.tasks[task_manager.current_task].real_entrypoint();
    task_manager.tasks[task_manager.current_task].status = DEAD_PROCESS;
    task_manager.num_tasks--;
    while(1);
}

void list_processes()
{
    int i;
    print_f("List of processes\nNumber name       status\n");
    for(i=0; i<143; i++)
    {
        if(task_manager.tasks[i].status != DEAD_PROCESS)
        {

            print_f("%6d %-10s ", i, task_manager.tasks[i].name);
            if (task_manager.tasks[i].status == SLEEPING_PROCESS)
                print("SLEEPING\n");
            else if (task_manager.tasks[i].status == ACTIVE_PROCESS)
                print("ACTIVE\n");

        }
    }
}