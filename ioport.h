#ifndef IOPORT_H
#define IOPORT_H

#include "types.h"

U8 insb(U16 address);
U16 insw(U16 address);
U32 inl(U16 address);

void outb(U16 address, U8 data);
void outw(U16 address, U16 data);
void outl(U16 address, U32 data);

#endif
