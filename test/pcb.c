#include "../h/pcb.h"


/*static pcb_t pcb_table[MAXPROC]; Tabella dei PCB*/

/*struct pcb_t pcbactive;*/

void insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem){
    if(*pcblist_p == NULL){
        /*Caso empty*/
        *pcblist_p = pcb_elem;
        pcb_elem->p_next = NULL;
        return;
    }
    else if ((*pcblist_p)->p_next == NULL) {
        /*Inserimento*/
        (*pcblist_p)->p_next = pcb_elem;
        pcb_elem->p_next = NULL;
    }
    else insertPCBList(&((*pcblist_p)->p_next), pcb_elem);
}

void initPcbs(void){
    static pcb_t pcb_table[MAXPROC]; /*Tabella dei PCB*/
    /*static pcb_t *pcbfree_h = NULL;*/
    for (int i = 0; i < MAXPROC; ++i) {
        insertPCBList(&pcbfree_h, &pcb_table[i]);
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

pcb_t* mkEmptyProcQ(){
    struct pcb_t* tmp = NULL;
    return tmp;
}

int emptyProcQ(pcb_t *tp){
    return tp == NULL;
}

void insertProcQ(pcb_t **tp, pcb_t* p){
    if ((*tp) == NULL) {(*tp) = p;}
    else {
        /*Pointer points to last Node*/
        pcb_t *last = (*tp)->p_prev;

        pcb_t *tmp = p;
        /*struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        new_node->data = value; /*Inserting the data*/

        /* setting up previous and next of new node*/
        tmp->p_next = *tp;
        tmp->p_prev = last;

        /* Update next and previous pointers of start
        and last.*/
        last->p_next = (*tp)->p_prev = tmp;

        /* Update start pointer*/
        *tp = tmp;
    }

}

pcb_t *headProcQ(pcb_t **tp){
    if (*tp == NULL) {return NULL;}
    else return *tp;
}
