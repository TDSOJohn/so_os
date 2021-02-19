#ifndef asl_h
#define asl_h

#include "pandos_const.h"
#include "pandos_types.h"


void    initASL();

pcb_t*  headBlocked(int *semAdd);
int     insertBlocked(int *semAdd, pcb_t *p);
pcb_t*  removeBlocked(int *semAdd);
pcb_t*  outBlocked(pcb_t *p);


#endif /*asl_h*/
