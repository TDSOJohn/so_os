#ifndef initial_h
#define initial_h


#include "pandos_const.h"
#include "pandos_types.h"
#include "../include/asl.h"
#include "../include/pcb.h"
#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include "../include/exceptions.h"
#include <umps3/umps/libumps.e>


extern int processCount;
extern int softBlockCount;
extern pcb_t* readyQueue;
extern pcb_t* currentProcess;
/* [0-7 dev 3 8-15 dev 4 16-23 dev 5 24-31 dev 6 32-39 dev7W 40-47 dev 7R + pseudoclock] */
extern int deviceSem[49];


#endif
