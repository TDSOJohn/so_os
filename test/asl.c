#include "../h/asl.h"


void insertSEMList(semd_t **semList, semd_t *sem_elem)
{
    if(*semList == NULL)
    {
        /*Caso empty*/
        *semList = sem_elem;
        sem_elem->s_next = NULL;
        return;
    }
    else if ((*semList)->s_next == NULL)
    {
        /*Inserimento*/
        (*semList)->s_next = sem_elem;
        sem_elem->s_next = NULL;
    }
    else
    {
        insertSEMList(&((*semList)->s_next), sem_elem);
    }
}


void initASL()
{
    static semd_t semdTable[MAXPROC];
    int i;
    for (i = 0; i < MAXPROC; i++)
    {
        insertSEMList(&semdFree_h, &semdTable[i]);
    }
    return;
}


pcb_t* headBlocked(int *semAdd)
{

}
