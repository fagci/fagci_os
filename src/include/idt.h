#ifndef __IDT_H
#define __IDT_H

#include "dev.h"
#include "memory.h"

void init_idt();
#define NUM_IDT_ENTRIES 256

#endif