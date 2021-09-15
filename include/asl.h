#ifndef asl_h
#define asl_h

#include "pandos_const.h"
#include "pandos_types.h"
#include "pcb.h"

/** Inizializza la lista dei semdFree in modo da contenere tutti gli elementi della semdTable.
* Questo metodo viene invocato una volta sola durante l’inizializzazione della struttura dati. */

void initASL();


/** Rimuove un semaforo da semdFree_h e lo alloca, inserendolo nella ASL.
*   Restituisce TRUE se semdFree è vuota.
*   Restituisce FALSE se ha successo.
*/

int allocFree(int *semAdd, pcb_t *p);


/**  Cerca il semaforo e lo restituisce.
*    Se non lo trova restituisce NULL
*/

semd_t *findSemd(int *semAdd);


/**	Rimuove dalla ASL il semd_t passato come parametro
*	e lo inserisce in testa alla semdFree
*/

void freeSemd(semd_t *current);


/** Restituisce (senza rimuovere) il puntatore al PCB che si trova in testa alla coda dei processi
* associata al SEMD con chiave semAdd. Ritorna NULL se il SEMD non compare nella ASL oppure
* se compare ma la sua coda dei processi è vuota.*/

pcb_t* headBlocked(int *semAdd);

/** Viene inserito il PCB puntato da p nella coda dei processi bloccati associata al SEMD con chiave
*semAdd. Se il semaforo corrispondente non e' presente nella ASL, alloca un nuovo SEMD dalla
*lista di quelli liberi (semdFree) e lo inserisce nella ASL, settando I campi in maniera opportuna (i.e.key e s_procQ).
*Se non e' possibile allocare un nuovo SEMD perché la lista di quelli liberi e' vuota, restituisce TRUE.
*In tutti gli altri casi, restituisce
*FALSE.
*/

int insertBlocked(int *semAdd, pcb_t *p);

/** Ritorna il primo PCB dalla coda dei processi bloccati (s_procq) associata al SEMD della
* ASL con chiave semAdd. Se tale descrittore non esiste nella ASL, restituisce NULL.
* Altrimenti, restituisce l’elemento rimosso. Se la coda dei processi bloccati per il semaforo
* diventa vuota, rimuove il descrittore corrispondente dalla ASL e lo inserisce nella
* coda dei descrittori liberi (semdFree_h).
*/

pcb_t*  removeBlocked(int *semAdd);

/** Rimuove il PCB puntato da p dalla coda del semaforo su cui è bloccato (indicato da p- >p_semAdd).
* Se il PCB non compare in tale coda, allora restituisce NULL (condizione di errore).
* Altrimenti, restituisce p.*/

pcb_t*  outBlocked(pcb_t *p);

#endif /*asl_h*/
