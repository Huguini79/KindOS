#ifndef PCB_H
#define PCB_H

// #include "gdt.h"
#include "types.h"

#define MAX_PROCESSES 64

typedef U32 pid_t;

enum State
{
	Zombie,
	Ready,
	Running,
	Swapper,
	Śleep,
	Runnable,
};

struct tss
{
	U32 prev_link;
	U32 esp0;
	U32 ss0;
	U32 esp1;
	U32 ss1;
	U32 esp2;
	U32 ss2;
	U32 cr3;
	U32 eip;
	U32 eflags;
	U32 eax;
	U32 ecx;
	U32 edx;
	U32 ebx;
	U32 esp;
	U32 ebp;
	U32 esi;
	U32 edi;
	U32 es;
	U32 cs;
	U32 ss;
	U32 ds;
	U32 fs;
	U32 gs;
	U32 ldtr;
	U32 iopb;
	
} __attribute__((packed));

/*
	* Process Control Block (PCB) structure
	* Note that we are using hardware multitasking, so we aren't saving and loading the registers manually, we are using the ljmp instruction to jump to the TSS of the process
*/
struct pcb
{
	pid_t pid;
	U32 alarm;
	U32 signal;
	enum State state;
	struct tss tss;
	
} __attribute__((packed));

extern struct pcb processes[MAX_PROCESSES];

struct pcb* createProcess(pid_t pid, U32 eip);
int exec(struct pcb* pcb);

#endif
