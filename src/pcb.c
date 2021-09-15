#include "../include/pcb.h"

HIDDEN pcb_t *pcbfree_h = NULL;

/** Inserisce il pcb puntato da pcb_elem nella lista puntata da pcblist_p*/

void insertPCBList(pcb_t **pcblist_p, pcb_t *pcb_elem)
{
    if(*pcblist_p == NULL)
    {
        /*Caso vuoto*/
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

/** Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table.
* Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della struttura dati.
*/

void initPcbs(void)
{
    static pcb_t pcb_table[MAXPROC];
    int i;
    for (i = 0; i<MAXPROC; i++){
    	insertPCBList(&pcbfree_h, &pcb_table[i]);
    }
}


/** Inserisce il PCB puntato da p nella lista dei PCB liberi (pcbFree_h)
*/

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

/** Restituisce NULL se la pcbFree_h è vuota.
* Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.
*/

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
        ptmp->p_semAdd = NULL;
        ptmp->p_time = 0;



        return ptmp;
    }
}

/** Crea una lista di PCB, inizializzandola come lista vuota (i.e. restituisce NULL).*/

pcb_t* mkEmptyProcQ()
{
    pcb_t* tmp = NULL;
    return tmp;
}

/** Restituisce TRUE se la lista puntata da tp è vuota, FALSE altrimenti.*/

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

/** Inserisce l’elemento puntato da p nella coda dei processi tp.
* La doppia indirezione su tp serve per poter inserire p come ultimo elemento della coda.*/

void insertProcQ(pcb_t **tp, pcb_t* p)
{
    if ((*tp) == NULL) /*Caso vuoto*/
    {
        (*tp) = p;
        p->p_next = p->p_prev = p;
        (*tp) = p;
        return;
    }
    else /*Caso generale*/
    {
        pcb_t *last = (*tp)->p_prev;

        pcb_t *tmp = p;

        tmp->p_next = *tp;
        tmp->p_prev = last;

        last->p_next = (*tp)->p_prev = tmp;

        *tp = tmp;
    }
}

/** Rimuove l’elemento piu’ vecchio dalla coda tp. Ritorna NULL se la coda è vuota,
* altrimenti ritorna il puntatore all’elemento rimosso dalla lista.*/

pcb_t* removeProcQ(pcb_t **tp)
{
    if (*tp == NULL)
    {
        return *tp;
    }
    else
    {
        struct pcb_t *curr = *tp, *prev_1 = NULL;

        /*Controlla se e' l'unico nodo della lista*/
        if (curr->p_next == *tp && prev_1 == NULL)
        {
            (*tp) = NULL;
        }
            /*Se ha piu' di un nodo controlla che sia il primo*/
        else
        {
            curr = (*tp)->p_prev;
            prev_1 = (*tp)->p_prev->p_prev;
            prev_1->p_next = *tp;
            (*tp)->p_prev = prev_1;
        }
        return curr;
    }
}

/** Rimuove il PCB puntato da p dalla coda dei processi puntata da tp.
* Se p non è presente nella coda, restituisce NULL (p può trovarsi in una posizione arbitraria della coda).
*/

pcb_t* outProcQ(pcb_t **tp, pcb_t* p)
{
  pcb_t* ret;
  pcb_t* temp;
if(((*tp) == NULL) || (p == NULL)) {
  return NULL;
}
/* only one thing in queue and it is what we want */
if((*tp) == p){

  if ((((*tp) -> p_next) == (*tp))) {
    ret = (*tp);
    (*tp) = mkEmptyProcQ();
    return ret;
  } else {
    (*tp)->p_prev->p_next = (*tp)->p_next;
    (*tp)->p_next->p_prev = (*tp)->p_prev;
    *tp = (*tp)->p_prev;
  }
  return p;
} else {
/* node is somewhere else, start at p_next */
temp = (*tp) -> p_next;
while(temp != (*tp)) {
  /* found node ? */
  if(temp == p){
    /* unleave node and return it */
    ret = temp;
    ret -> p_prev -> p_next = ret -> p_next;
    ret -> p_next -> p_prev = ret -> p_prev;
    ret -> p_next = NULL;
    ret -> p_prev = NULL;
    return ret;
  }
    temp = temp -> p_next;
  }
  /* node not in list here */
  return NULL;
}
}


/** Restituisce l’elemento in fondo alla coda dei processi tp, SENZA RIMUOVERLO.
* Ritorna NULL se la coda non ha elementi.*/

pcb_t *headProcQ(pcb_t *tp)
{
    return (tp->p_prev);
}

/**ALBERI**/

/** Restituisce TRUE se il PCB puntato da p non ha figli, FALSE altrimenti.*/

int emptyChild(pcb_t *p)
{
    return p->p_child == NULL;
}


/** Inserisce il PCB puntato da p come figlio del PCB puntato da prnt.*/
void insertChild(pcb_t *prnt, pcb_t *p)
{
    p->p_prnt=prnt;
    if (emptyChild(prnt)) /*Caso nessun figlio*/
    {
        prnt->p_child = p;
    }
    else /*Caso generale*/
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

/** Rimuove il primo figlio del PCB puntato da p. Se p non ha figli, restituisce NULL.*/
pcb_t* removeChild(pcb_t *p)
{
    if(p->p_child==NULL) return NULL;
    else if(p->p_child->p_next_sib== NULL) /*Caso unico figlio*/
    {
        struct pcb_t *curr = p->p_child;
        p->p_child->p_prnt=NULL;
        p->p_child=NULL;
        return curr;
    } else /*Caso generale*/
    {
        struct pcb_t *curr = p->p_child;
        p->p_child= p->p_child->p_next_sib;
        curr->p_prnt=NULL;
        curr->p_next_sib=NULL;
        p->p_child->p_prev_sib=NULL;
        return curr;
    }
}


/** Rimuove il PCB puntato da p dalla lista dei figli del padre.
* Se il PCB puntato da p non ha un padre, restituisce NULL, altrimenti restituisce l’elemento
* rimosso (cioè p). A differenza della removeChild, p può trovarsi in una posizione arbitraria
* (ossia non è necessariamente il primo figlio del padre). */

pcb_t* outChild(pcb_t *p)
{
    if(p->p_prnt==NULL) return NULL;
    else if(p->p_prnt->p_child== p) return (removeChild(p->p_prnt)); /*Se p e' il primo figlio richiama removeChild()*/
    else if(p->p_next_sib== NULL) /*Caso l'ultimo figlio*/
    {
        struct pcb_t *curr = p;
        p->p_prev_sib->p_next_sib = NULL;
        p->p_prev_sib= NULL;
        p->p_prnt=NULL;
        return curr;
    }
    else /*Caso generale*/
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
