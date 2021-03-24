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
pcb_t*  headProcQ(pcb_t *tp);
pcb_t*  outProcQ(pcb_t **tp, pcb_t* p);
pcb_t*  removeProcQ(pcb_t **tp);


int     emptyChild(pcb_t *p);
void    insertChild(pcb_t *prnt, pcb_t *p);
pcb_t*  removeChild(pcb_t *p);
pcb_t*  outChild(pcb_t *p);

#endif /*pcb_h*/
