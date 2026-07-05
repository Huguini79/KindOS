#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "pcb.h"

void setTSSDescriptor(struct pcb* pcb);
void GdtInstall();

#endif
