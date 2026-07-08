#ifndef CALLOUTS_H
#define CALLOUTS_H

#include "types.h"

struct callout
{
	U32 pid;
	U32 ticks;
	void* handler;
	
} __attribute__((packed));

extern struct callout callouts[64];

#endif
