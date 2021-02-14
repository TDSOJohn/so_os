#ifndef pcb_h
#define pcb_h

#include "pandos_const.h"
#include "pandos_types.h"


void    initPcbs(void);
void    insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem);
void    freePcb(pcb_t *p);

pcb_t*  allocPcb();

pcb_t*  mkEmptyProcQ();
int     emptyProcQ(pcb_t *tp);
void    insertProcQ(pcb_t **tp, pcb_t *p);
pcb_t*  headProcQ(pcb_t **tp);

#endif /*pcb_h*/
