#ifndef pcb_h
#define pcb_h

#include "const.h"
#include "types.h"


typedef struct pcb_t
{
    struct pcb_t    *p_next;
    struct pcb_t    *p_prev;

    struct pcb_t    *p_prnt,
                    *p_child,
                    *p_next_sib,
                    *p_prev_sib;
} pcb_t, *pcb_PTR;


pcb_t *pcbfree_h = NULL;

void    initPcbs(void);
void    insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem);
void    freePcb(pcb_t *p);

pcb_t*  allocPcb();

pcb_t*  mkEmptyProcQ();
int     emptyProcQ(pcb_t *tp);
void    insertProcQ(pcb_t **tp, pcb_t *p);
pcb_t*  headProcQ(pcb_t **tp);

#endif /*pcb_h*/
