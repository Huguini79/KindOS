#include "types.h"
#include "gdt.h"
#include "pcb.h"

struct pcb processes[64] = {0};

void blank()
{
	
}

struct pcb* createProcess(pid_t pid, U32 eip)
{
		struct pcb* newProcess = &processes[pid];
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
