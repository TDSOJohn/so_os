#include "../h/asl.h"
#include "../h/pcb.h"

HIDDEN semd_t *semd_h, *semdFree_h = NULL;

HIDDEN void insertSEMList(semd_t **semList, semd_t *sem_elem){
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

HIDDEN int emptySEMList(semd_t **semlist_p){
	if(*semlist_p == NULL)
		return 1;
	else
		return 0;
}

HIDDEN semd_t* getSemd_rec(semd_t **semd_h, int* semAdd){
	if(*semd_h == NULL){
		return NULL; /*Il semaforo non esiste nella ASL*/
	}

	if((*semd_h)->s_semAdd == semAdd){

		return *semd_h;
	}

	else
		return getSemd_rec(&((*semd_h)->s_next), semAdd);
}


/*Cerca nella ASL il semaforo con chiave semAdd e restituisce un puntatore ad esso*/
HIDDEN semd_t* getSemd(int *semAdd){
	return getSemd_rec(&semd_h, semAdd);
}


/*Estrae un SEM dalla lista dei semdFree e restituisce un puntatore a esso*/
HIDDEN semd_t *allocSem(){
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


int insertBlocked(int *semAdd, pcb_t *p){
		semd_t *semd_target = getSemd(semAdd);
		if(semd_target == NULL){
			/*Il semaforo non esiste nella ASL*/
			semd_target = allocSem(); /*semd_target ora punta al SEMD tolto dalla semdFree. Se semdFree è vuota, semd_target == NULL*/
			if (semd_target == NULL) /*Se la semdFree è vuota restituisco TRUE*/
				return 1;
			semd_target->s_semAdd = semAdd;
			insertSEMList(&semd_h, semd_target); /*Alloco semd_target nella ASL*/
			p->p_semAdd = semAdd;
			if(*semAdd <= 0)
				insertProcQ(&(semd_target->s_procQ), p); /*Inserisco p nella coda di processi bloccati di semd_target*/
			return 0;
		}
		else{
			/*Il semaforo esiste già nella ASL*/
			insertProcQ(&(semd_target->s_procQ), p);
			p->p_semAdd = semAdd;
			return 0;
		}
}



/*Rimuove dalla ASL il semaforo puntato da sem*/
HIDDEN semd_t *deAllocSem(semd_t **semd_h, semd_t *sem){
	if(*semd_h == sem){
		semd_t *removed = sem;
		*semd_h = sem->s_next;

		removed->s_semAdd = NULL;
		removed->s_next = NULL;
		insertSEMList(&semdFree_h, removed); /*Reinserisce il semaforo nella semdFree*/
		return removed;
	}
	else
		return deAllocSem(&((*semd_h)->s_next), sem);
}

/*Ritorna il primo PCB dalla coda dei processi bloccati(s_ProcQ) associata al SEMD della ASL con chiave semAdd.
Se tale descrittore non esiste nella ASL, restituisce NULL.
Altrimenti,restituisce l’elemento rimosso.
Se la coda dei processi bloccati per il semaforo diventa vuota, rimuove il descrittore del semaforo corrispondente dalla ASL e lo inserisce nella coda dei semafori liberi (semdFree).*/
pcb_t* removeBlocked(int *semAdd){
	semd_t *semd_target = getSemd(semAdd);
	if(semd_target){
		/*Il semaforo è nella ASL*/
		pcb_t *removed = removeProcQ(&(semd_target->s_procQ)); /*Rimuovo il primo PCB della sua coda di processi*/
		if (semd_target->s_procQ == NULL)
			deAllocSem(&semd_h, semd_target);
		return removed;
	}
	else
		return NULL;
}

/*Restituisce il puntatore al pcb del primo processo bloccato sul semaforo, senza deaccordarlo.
Se il semaforo non esiste restituisce NULL.*/
pcb_t* headBlocked(int *semAdd){
	semd_t *semd_target = getSemd(semAdd);

	if (semd_target == NULL || semd_target->s_procQ == NULL)
		return NULL;
	else
		return headProcQ(semd_target->s_procQ);
}
/*Rimuove il pcb puntato da p dalla coda dei processi sul semaforo a lui associato (p->p_semkey) sulla ASL.
Se il processo non risulta bloccato (cioè è un errore), restituisce NULL, altrimenti restituisce il puntatore al processo*/
pcb_t* outBlocked(pcb_t *p){
	semd_t *semd_target = getSemd(p->p_semAdd);
	pcb_t *blocked = NULL;
	if(semd_target)
		blocked = outProcQ(&(semd_target->s_procQ), p);
	/*Se p non esiste nella coda del semaforo*/
	return blocked;
}
