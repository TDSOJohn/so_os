#include "../include/asl.h"
#include "../include/pcb.h"

HIDDEN semd_t *semd_h, *semdFree_h = NULL;

/**  Cerca il semaforo e lo restituisce.
*    Se non lo trova restituisce NULL
*/

semd_t *findSemd(int *semAdd){
	semd_t *i = semd_h;
	semd_t *tmp = NULL;
	if(semd_h != NULL){
		while(i -> s_semAdd < semAdd && i -> s_next != NULL){
	        i = i -> s_next;
	    }
	    if(i->s_semAdd == semAdd){
	    	tmp = i;
	    }
	}
    return tmp;
}

/**	Rimuove dalla ASL il semd_t passato come parametro
*	e lo inserisce in testa alla semdFree
*/
void freeSemd(semd_t *current){
	semd_t *i = semd_h;
	/* Toglie dalla ASL */
	if(i != current){
		while(i -> s_next != current){
			i = i -> s_next;
		}
		i -> s_next = current -> s_next;
	} else {
		semd_h = semd_h -> s_next;
	}
	/* Aggiunge in testa alla semdFree */
	current -> s_next = semdFree_h;
	semdFree_h = current;
}

/** Rimuove un semaforo da semdFree_h e lo alloca, inserendolo nella ASL.
*   Restituisce TRUE se semdFree è vuota.
*   Restituisce FALSE se ha successo.
*/
int allocFree(int *semAdd, pcb_t *p){
	/* Alloco un nuovo semaforo per metterlo nell'ASL */
	int result = FALSE;
	semd_t *i = NULL;
	if(semdFree_h != NULL){
		semd_t *alloc = findSemd(semAdd);
		if(alloc==NULL && semdFree_h != NULL){
			alloc = semdFree_h;
        	semdFree_h = semdFree_h -> s_next;
       		alloc -> s_next = NULL;
        	alloc -> s_semAdd = semAdd;
        	alloc -> s_procQ = mkEmptyProcQ();
        	/* Coda vuota  */
        	if(semd_h == NULL){
        		semd_h = alloc;
        	} else {
        		if(alloc->s_semAdd < semd_h->s_semAdd){
					/* Inserimento in testa */
					alloc -> s_next = semd_h;
					semd_h = alloc;
				} else {
					/* Scorro la ASL */
					i = semd_h;
					while(i -> s_next != NULL && alloc->s_semAdd > (i -> s_next)->s_semAdd){
						i = i -> s_next;
					}
					if(i -> s_next == NULL){
						/* Inserimento in coda */
						i -> s_next = alloc;
					} else {
						/* Inserimento prima del semAdd maggiore di alloc */
						alloc -> s_next = i -> s_next;
						i -> s_next = alloc;
					}
				}
        		}
		}
        p -> p_semAdd = semAdd;
        insertProcQ(&(alloc -> s_procQ), p);
    } else {
    	result = TRUE;
    }
    return result;
}

/** Viene inserito il PCB puntato da p nella coda dei processi bloccati associata al SEMD con chiave
*semAdd. Se il semaforo corrispondente non e' presente nella ASL, alloca un nuovo SEMD dalla
*lista di quelli liberi (semdFree) e lo inserisce nella ASL, settando I campi in maniera opportuna (i.e.key e s_procQ).
*Se non e' possibile allocare un nuovo SEMD perché la lista di quelli liberi e' vuota, restituisce TRUE.
*In tutti gli altri casi, restituisce
*FALSE.
*/

int insertBlocked(int *semAdd, pcb_t *p){
    int result = FALSE;
    semd_t *i = findSemd(semAdd);
    if(i == NULL){
		/* ASL vuota, alloco un semaforo */
        result = allocFree(semAdd, p);
    } else {
		/* Il parametro è nella ASL */
		p -> p_semAdd = semAdd;
		insertProcQ(&(i -> s_procQ), p);
    }
    return result;
}


/** Ritorna il primo PCB dalla coda dei processi bloccati (s_procq) associata al SEMD della
* ASL con chiave semAdd. Se tale descrittore non esiste nella ASL, restituisce NULL.
* Altrimenti, restituisce l’elemento rimosso. Se la coda dei processi bloccati per il semaforo
* diventa vuota, rimuove il descrittore corrispondente dalla ASL e lo inserisce nella
* coda dei descrittori liberi (semdFree_h).
*/


pcb_t *removeBlocked(int *semAdd){
    pcb_t *result = NULL;
   	semd_t *i = findSemd(semAdd);
	if(i != NULL){
		result = removeProcQ(&(i -> s_procQ));
		/* Se la sua coda diventa vuota */
		if(emptyProcQ(i ->s_procQ)){
			freeSemd(i);
		}
	}
	return result;
}


/** Rimuove il PCB puntato da p dalla coda del semaforo su cui è bloccato (indicato da p- >p_semAdd).
* Se il PCB non compare in tale coda, allora restituisce NULL (condizione di errore).
* Altrimenti, restituisce p.*/


pcb_t *outBlocked(pcb_t *p){
    pcb_t *result = NULL;
    semd_t *i = findSemd(p->p_semAdd);
    if(i != NULL){
		result = outProcQ(&(i->s_procQ), p);
		/* Se la sua coda diventa vuota */
		if(emptyProcQ(i ->s_procQ)){
			freeSemd(i);
		}
	}
    return result;
}

/** Restituisce (senza rimuovere) il puntatore al PCB che si trova in testa alla coda dei processi
* associata al SEMD con chiave semAdd. Ritorna NULL se il SEMD non compare nella ASL oppure
* se compare ma la sua coda dei processi è vuota.*/

pcb_t *headBlocked(int *semAdd){
    pcb_t *result = NULL;
    semd_t *i = findSemd(semAdd);
    if(i != NULL){
        result = headProcQ(i -> s_procQ);
    }
    return result;
}

/** Inizializza la lista dei semdFree in modo da contenere tutti gli elementi della semdTable.
* Questo metodo viene invocato una volta sola durante l’inizializzazione della struttura dati. */

void initASL(){
    static semd_t semdTable[MAXPROC];
    int i;
    semd_h = NULL;
    semdFree_h = &semdTable[0];
    for(i=1; i < MAXPROC; i++){
        semdTable[i-1].s_next = &semdTable[i];
    }
    semdTable[MAXPROC-1].s_next = NULL;
}
