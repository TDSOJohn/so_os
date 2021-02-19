#include "../h/asl.h"
#include "../h/pcb.h"


semd_t *semd_h, *semdFree_h;

void insertSEMList(semd_t **semList, semd_t *sem_elem){
    if(*semList == NULL){
        /*Caso empty*/
        *semList = sem_elem;
        sem_elem->s_next = NULL;
        return;
    }
    else if ((*semList)->s_next == NULL) {
        /*Inserimento*/
        (*semList)->s_next = sem_elem;
        sem_elem->s_next = NULL;
    }
    else {
        insertSEMList(&((*semList)->s_next), sem_elem);
    }
}


void initASL() {
    static semd_t semdTable[MAXPROC];
    int i;
    for (i = 0; i < MAXPROC; i++) {
        insertSEMList(&semdFree_h, &semdTable[i]);
    }
    return;
}

semd_t *findSemd(int *semAdd) {
    semd_t *i = semd_h;
    semd_t *tmp = NULL;
    if (semd_h != NULL) {
        while (i->s_semAdd < semAdd && i->s_next != NULL) {
            i = i->s_next;
            if (i->s_semAdd == semAdd) {
                tmp = i;
            }
        }
    }
    return tmp;
}

/*Controlla se la lista di pcb_t* puntata da pcblist_p è vuota*/
int emptySEMList(semd_t **semlist_p){
    if(*semlist_p == NULL)
        return 1;
    else
        return 0;
}


/*Estrae un SEM dalla lista dei semdFree e restituisce un puntatore a esso*/
semd_t *allocSem(){
    if(emptySEMList(&semdFree_h))
        return NULL;
    else{
        semd_t *ptemp = semdFree_h;
        semdFree_h = semdFree_h->s_next;

        ptemp->s_next = NULL;
        ptemp->s_semAdd = NULL;
        ptemp->s_procQ = NULL;
        return ptemp;
    }
}

int insertBlocked(int *semAdd,pcb_t *p){
    semd_t *tmp = findSemd(semAdd);
    if (tmp == NULL) {
        tmp = allocSem();
        if (tmp == NULL) {
            return 1;
        }
        tmp -> s_semAdd = semAdd;
        insertSEMList(&semd_h, tmp);
        p->p_semAdd = semAdd;
        if (*semAdd <= 0) {
            insertProcQ(&(tmp->s_procQ), p);
        }
        return 0;
    } else {
        insertProcQ(&(tmp -> s_procQ), p);
        p->p_semAdd = semAdd;
        return 0;
    }
}


pcb_t* headBlocked(int *semAdd)
{

}

pcb_t*  removeBlocked(int *semAdd)
{}

pcb_t*  outBlocked(pcb_t *p)
{}
