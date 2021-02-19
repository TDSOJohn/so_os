#include "../h/pcb.h"



pcb_t *pcbfree_h = NULL;


void insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem)
{
    if(*pcblist_p == NULL)
    {
        /*Caso empty*/
        *pcblist_p = pcb_elem;
        pcb_elem->p_next = NULL;
        return;
    }
    else if ((*pcblist_p)->p_next == NULL)
    {
        /*Inserimento*/
        (*pcblist_p)->p_next = pcb_elem;
        pcb_elem->p_next = NULL;
    }
    else insertPCBList(&((*pcblist_p)->p_next), pcb_elem);
}


void initPcbs_rec(int count)
{
    static pcb_t pcb_table[MAXPROC];

    if (count>=MAXPROC) /*0...MAXPROC-1*/
        return;
    insertPCBList(&pcbfree_h, &pcb_table[count]);
    count++;
    initPcbs_rec(count);
}


void initPcbs(void)
{
    initPcbs_rec(0);
}


void freePcb(pcb_t *p)
{
    p->p_next = NULL;
    p->p_prev = NULL;
    p->p_child = NULL;
    p->p_prnt = NULL;
    p->p_next_sib = NULL;
    p->p_prev_sib = NULL;
    insertPCBList(&pcbfree_h, p);
}


pcb_t *allocPcb()
{
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


pcb_t* mkEmptyProcQ()
{
    pcb_t* tmp = NULL;
    return tmp;
}

int emptyProcQ(pcb_t *tp)
{
    if(tp == NULL)
    {
        return 1;
    } else
    {
        return 0;
    }
    return tp == NULL;
}


void insertProcQ(pcb_t **tp, pcb_t* p)
{
    if ((*tp) == NULL)
    {
        (*tp) = p;
        p->p_next = p->p_prev = p;
        (*tp) = p;
        return;
    }
    else
    {
        pcb_t *last = (*tp)->p_prev;

        pcb_t *tmp = p;

        tmp->p_next = *tp;
        tmp->p_prev = last;

        last->p_next = (*tp)->p_prev = tmp;

        *tp = tmp;
    }
}


pcb_t* removeProcQ(pcb_t **tp)
{
    if (*tp == NULL)
    {
        return *tp;
    }
    else
    {
        /*// Declare two pointers and initialze them*/
        struct pcb_t *curr = *tp, *prev_1 = NULL;

        /*// Check if node is the only node in list*/
        if (curr->p_next == *tp && prev_1 == NULL)
        {
            (*tp) = NULL;
        }
            /*// If list has more than one node,
            // check if it is the first node*/
        else
        {
            curr = (*tp)->p_prev;
            prev_1 = (*tp)->p_prev->p_prev;

            /*// Adjust the pointers of prev_1 and start node*/
            prev_1->p_next = *tp;
            (*tp)->p_prev = prev_1;
        }
        return curr;
    }
}


pcb_t* outProcQ(pcb_t **tp, pcb_t* p)
{
    pcb_t *iterator = (*tp);
    pcb_t *removed = NULL;
    if(p != NULL && !emptyProcQ((*tp)))
    {
		while(iterator -> p_next != p && iterator -> p_next != (*tp))
        {
			/* Scorro in avanti */
			iterator = iterator -> p_next;
		}
		if( iterator -> p_next == p )
        {
			removed = iterator -> p_next;
            /* Se la coda ha un solo elemento */
			if(iterator -> p_next == iterator)
            {
				*(tp) = NULL;
			} else
            {
				iterator -> p_next = removed -> p_next;
				removed -> p_next -> p_prev = iterator;
			}
		}
	}
    return removed;
}


pcb_t *headProcQ(pcb_t *tp)
{
    return (tp->p_prev);
}

/**ALBERI**/

int emptyChild(pcb_t *p)
{
    return p->p_child == NULL;
}

void insertChild(pcb_t *prnt, pcb_t *p)
{
	p->p_prnt=prnt;
    if (emptyChild(prnt))
    {
        prnt->p_child = p;
    }
    else
    {
    	pcb_t *last = prnt -> p_child;
    	while (last -> p_next_sib != NULL)
        {
    		last = last -> p_next_sib;
    	}
    	last -> p_next_sib = p;
    	p -> p_prev_sib = last;
    	p -> p_next_sib = NULL;
    }
}


pcb_t* removeChild(pcb_t *p)
{
    if(p->p_child==NULL) return NULL;
    else if(p->p_child->p_next_sib== NULL)
    {
        struct pcb_t *curr = p->p_child;
        p->p_child->p_prnt=NULL;
        p->p_child=NULL;
        return curr;
    } else
    {
        struct pcb_t *curr = p->p_child;
        p->p_child= p->p_child->p_next_sib;
        curr->p_prnt=NULL;
        curr->p_next_sib=NULL;
        p->p_child->p_prev_sib=NULL;
        return curr;
    }
}


pcb_t* outChild(pcb_t *p)
{
    if(p->p_prnt==NULL) return NULL;
    else if(p->p_prnt->p_child== p) return (removeChild(p->p_prnt));
    else if(p->p_next_sib== NULL)
    {
        struct pcb_t *curr = p;
        p->p_prev_sib->p_next_sib = NULL;
        p->p_prev_sib= NULL;
        p->p_prnt=NULL;
        return curr;
    }

    else
    {
    	struct pcb_t *curr = p;
        p->p_prev_sib->p_next_sib = p->p_next_sib;
        p->p_next_sib->p_prev_sib=p->p_prev_sib;
        p->p_prev_sib=NULL;
        p->p_next_sib=NULL;
        p->p_prnt=NULL;
        return curr;
    }
}
