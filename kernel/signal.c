#include "types.h"
#include "pcb.h"
#include "signal.h"

void sendsig(struct pcb* pcb, U32 signal)
{
    pcb->signal |= 1U << (signal);
}

void psig(struct pcb* pcb)
{
    if (pcb->signal & (1U << SIGTERM))
    {
        printk("* SIGTERM signal for process with pid=");
        char buf[16];
        itoa(pcb->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        pcb->signal &= ~(1U << SIGTERM);
    
    } else if (pcb->signal & (1U << SIGINT))
    {
        printk("* SIGINT signal for process with pid=");
        char buf[16];
        itoa(pcb->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        pcb->signal &= ~(1U << SIGINT);

    } else if (pcb->signal & (1U << SIGALRM))
    {
        printk("* SIGALRM signal for process with pid=");
        char buf[16];
        itoa(pcb->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        pcb->signal &= ~(1U << SIGALRM);
    }
}