#include <stdio.h>
#include "pandos_const.h"
#include "pandos_types.h"

/*static pcb_t pcb_table[MAXPROC]; Tabella dei PCB*/
pcb_t *pcbfree_h = NULL;
/*struct pcb_t pcbactive;*/

void insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem){
    if(*pcblist_p == NULL){
        /*Caso empty*/
        *pcblist_p = pcb_elem;
        pcb_elem->p_next = NULL;
        printf("Primo if\n");
        return;
    }
    else if ((*pcblist_p)->p_next == NULL) {
        /*Inserimento*/
        (*pcblist_p)->p_next = pcb_elem;
        pcb_elem->p_next = NULL;
        printf("Altri if\n");
    }
    else insertPCBList(&((*pcblist_p)->p_next), pcb_elem);
}

void initPcbs(void){
    static pcb_t pcb_table[MAXPROC]; /*Tabella dei PCB*/
    //static pcb_t *pcbfree_h = NULL;
    for (int i = 0; i < MAXPROC; ++i) {
        insertPCBList(&pcbfree_h, &pcb_table[i]);
        printf("i = %d\n", i);
    }
}

void freePcb(pcb_t *p) {
    p->p_next = NULL;
    p->p_prev = NULL;
    p->p_child = NULL;
    p->p_prnt = NULL;
    p->p_next_sib = NULL;
    p->p_prev_sib = NULL;
    insertPCBList(&pcbfree_h, p);
}

pcb_t *allocPcb() {
    if (pcbfree_h == NULL) return NULL;
    else{
        pcb_t *ptmp = pcbfree_h;
        pcbfree_h = pcbfree_h->p_next;

        ptmp->p_next = NULL;
        ptmp->p_prev = NULL;
        ptmp->p_child = NULL;
        ptmp->p_prnt = NULL;
        ptmp->p_next_sib = NULL;
        ptmp->p_prev_sib = NULL;

        return ptmp;
    }
}
