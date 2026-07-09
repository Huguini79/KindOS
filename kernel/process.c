#include "types.h"
#include "gdt.h"
#include "pcb.h"
#include "printk.h"
#include "string.h"
#include "signal.h"

struct pcb* current = &processes[0];
struct pcb* next = &processes[0];
struct pcb processes[64] = {0};
struct pcb zombie_processes[64] = {0};

void deleteProcess(struct pcb* pcb)
{
	pcb->alarm = 0;
	pcb->signal = 0;
	pcb->tss.eax = 0;
	pcb->tss.ecx = 0;
	pcb->tss.edx = 0;
	pcb->tss.ebx = 0;
	pcb->tss.edi = 0;
	pcb->tss.esi = 0;
	pcb->tss.esp = 0;
	pcb->tss.eflags = 0;
	pcb->tss.eip = 0;
	pcb->tss.ebp = 0;
	pcb->tss.iopb = 0;
}

void blank()
{

}

int searchProcesses()
{
	int c = 0;
	for (int i = 1; i < 64; ++i)
	{
		if (processes[i].tss.eip != 0)
		{
			c++;
		}
	}

	return c;
}

struct pcb* createProcess(pid_t pid, U32 eip)
{
		struct pcb* newProcess = &processes[pid];
		newProcess->name = "new process";
		newProcess->counter = 64 - pid;
		newProcess->pid = pid;
		newProcess->alarm = 0;
		newProcess->signal = 0;
		newProcess->state = Ready;
		newProcess->tss.esp0 = 0x600000;
		newProcess->tss.ss0 = 0x10;

		newProcess->tss.eax = 0;
		newProcess->tss.ecx = 0;
		newProcess->tss.edx = 0;
		newProcess->tss.ebx = 0;

		newProcess->tss.eip = eip;

		U32* stack = (U32*)0x3FF00 + pid * 8192;

		stack--;
		*stack = blank;
		
		newProcess->tss.esp = (U32)stack;
		newProcess->tss.ebp = 0;
		
		newProcess->tss.cs = 0x08;
		newProcess->tss.ss = 0x10;
		newProcess->tss.ds = 0x10;
		newProcess->tss.es = 0x10;
		newProcess->tss.fs = 0x10;
		newProcess->tss.gs = 0x10;
		newProcess->tss.eflags = 0x202;
		newProcess->tss.iopb = 0x80000000;

		setTSSDescriptor(newProcess);

		return newProcess;
}

void markAsZombie(struct pcb* pcb)
{
	pcb->state = Zombie;
	memset(&zombie_processes[current->pid], pcb, sizeof(struct pcb));
}

void yield()
{
	if (searchProcesses() > 1)
	{
		if (processes[current->pid+1].tss.eip != 0 && processes[current->pid+1].state != Zombie)
		{
			next = &processes[current->pid+1];

		} else
		{
			next = &processes[1];
		}

		current->state = Ready;
		current = next;
		current->state = Running;

		if (current->signal != 0)
		{
			psig(current);
		}

		put_cxy('C', 60, 0);
		put_cxy('U', 61, 0);
		put_cxy('R', 62, 0);
		put_cxy('R', 63, 0);
		put_cxy('E', 64, 0);
		put_cxy('N', 65, 0);
		put_cxy('T', 66, 0);
		put_cxy(':', 67, 0);
		put_cxy(' ', 68, 0);
		put_cxy('P', 69, 0);
		put_cxy('I', 70, 0);
		put_cxy('D', 71, 0);
		put_cxy('=', 72, 0);
		char pid_buf[16];
		itoa(current->pid, pid_buf, 10);
		put_cxy(pid_buf[0], 73, 0);
		
		exec(current);
	}
}

int exec(struct pcb* pcb)
{
	if (pcb->tss.eip != 0)
	{
		U16 selector = (pcb->pid + 4) * 8;

		volatile struct
		{
			U32 offset;
			U16 selector;
			
		} __attribute__((packed)) _tmp;

		_tmp.offset = 0;
		_tmp.selector = selector;

		__asm__ volatile ("ljmp %0" :: "m"(_tmp));
		
		return 0;
	} else
	{
		return -1;
	}
}

void ps()
{
	char buf[16];
	printk("\nPID          CMD          STATE\n");
	for (int i = 0; i < 64; ++i)
	{
		if (processes[i].state == Running || processes[i].state == Ready || processes[i].state == Zombie && processes[i].tss.eip != 0)
		{
			itoa(processes[i].pid, buf, 10);
			printk(buf);
			printk("          ");
			printk(processes[i].name);
			printk("          ");
			if (processes[i].state == Running)
			{
				printk("Running");
				
			} else if (processes[i].state == Ready)
			{
				printk("Ready");

			} else
			{
				printk("Zombie");
			}
			printk("\n");
		}
	}
}
