#ifndef vmSupport_h
#define vmSupport_h

#include "pandos_const.h"
#include "pandos_types.h"
#include "../include/pcb.h"
#include "../include/asl.h"
#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include "../include/initial.h"
#include <umps3/umps/libumps.e> 
#include "../include/initProc.h"

void initSwapStructs();
void pager();
int getFrame();

#endif