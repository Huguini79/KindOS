#include "callouts.h"
#include "types.h"
#include "pcb.h"

struct callout callouts[64] = {0};

int searchForFree()
{
    for (int i = 0; i < 64; ++i)
    {
        if (callouts[i].ticks == 0)
        {
            return i;
        }
    }

    return -1;
}

void alarm(U32 s)
{
    /*
        * We have 100 HZ
        * This means the clock executes every 10 milliseconds
        * The clock generates a tick when he is active
        * So we use a basic formula for calculating the cpu ticks from the milliseconds: ms/10
        * Ms divided by each 10 milliseconds the clock generates a tick, tells us the cpu ticks
    */
   U32 ms = s * 1000;
    current->state = Zombie;
    U32 cpu_ticks = ms / 10;
    // int n = searchForFree();
    // if (n != 1)
    // {
    callouts[current->pid].pid = current->pid;
    callouts[current->pid].ticks = cpu_ticks;
    callouts[current->pid].handler = current->tss.eip;
    // }
}