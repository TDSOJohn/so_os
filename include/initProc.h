#ifndef initProc_h
#define initProc_h

#include "pandos_const.h"
#include "pandos_types.h"
#include "../include/pcb.h"
#include "../include/asl.h"
#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include "../include/initial.h"
#include <umps3/umps/libumps.e> 
#include "../include/vmSupport.h"

void testo();
void initSuppStructs (int id, memaddr topStack);
void initUserProc(int id);

#endif