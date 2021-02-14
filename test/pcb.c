#include "../h/pcb.h"



pcb_t pcb_table[MAXPROC];
pcb_t *pcbfree_h = NULL;


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

void initPcbs_rec(int count){
    if (count>=MAXPROC) /*0...MAXPROC-1*/
        return;
    insertPCBList(&pcbfree_h, &pcb_table[count]);
    count++;
    initPcbs_rec(count);
}

void initPcbs(void){
    initPcbs_rec(0);
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
    if (pcbfree_h == NULL)
        return NULL;
    else
    {
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
    pcb_t* tmp = NULL;
    return tmp;
}

int emptyProcQ(pcb_t *tp){
    return tp == NULL;
}

void insertProcQ(pcb_t **tp, pcb_t* p){
    if ((*tp) == NULL) {(*tp) = p;}
    else {
        /*// Pointer points to last Node*/
        pcb_t *last = (*tp)->p_prev;

        pcb_t *tmp = p;
/*        //struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        //new_node->data = value; // Inserting the data

        // setting up previous and next of new node*/
        tmp->p_next = *tp;
        tmp->p_prev = last;

        /*// Update next and previous pointers of start
        // and last.*/
        last->p_next = (*tp)->p_prev = tmp;

        /*// Update start pointer*/
        *tp = tmp;
    }

}

pcb_t* removeProcQ(pcb_t **tp)
{
    if (*tp == NULL) {
        return *tp;
    }
    else {
        /*// Declare two pointers and initialze them*/
        struct pcb_t *curr = *tp, *prev_1 = NULL;

        /*// Check if node is the only node in list*/
        if (curr->p_next == *tp && prev_1 == NULL) {
            (*tp) = NULL;
        }
            /*// If list has more than one node,
            // check if it is the first node*/
        else {
            curr = (*tp)->p_prev;
            prev_1 = (*tp)->p_prev->p_prev;

            /*// Adjust the pointers of prev_1 and start node*/
            prev_1->p_next = *tp;
            (*tp)->p_prev = prev_1;
        }
        return curr;
    }
}

struct pcb_t * deleteNodeOut(struct pcb_t** tp)
{
    if (*tp == NULL) {
        return *tp;
    }
    else {
        /*// Declare two pointers and initialze them*/
        struct pcb_t *curr = *tp, *prev_1 = NULL;

        /*// Check if node is the only node in list*/
        if (curr->p_next == *tp && prev_1 == NULL) {
            (*tp) = NULL;
        }
            /*// If list has more than one node,
            // check if it is the first node*/
        else {
            prev_1 = (*tp)->p_prev;

            /*// Move start ahead*/
            *tp = (*tp)->p_next;

            /*// Adjust the pointers of prev_1 and start node*/
            prev_1->p_next = *tp;
            (*tp)->p_prev = prev_1;

        }
        return curr;
    }
}

pcb_t* outProcQ(pcb_t **tp, pcb_t* p){
    if (tp == NULL)
        return NULL;

    else if((*tp) == p)
        return deleteNodeOut(tp);

    else
        return outProcQ(&((*tp)->p_next), p);
}


pcb_t *headProcQ(pcb_t **tp){
    if (*tp == NULL) return NULL;
    else return *tp;
}
