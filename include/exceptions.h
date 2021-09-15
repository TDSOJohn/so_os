#ifndef exceptions_h
#define exceptions_h

#include "pandos_const.h"
#include "pandos_types.h"
#include "../include/pcb.h"
#include "../include/asl.h"
#include "../include/scheduler.h"
#include "../include/interrupts.h"
#include "../include/initial.h"
#include <umps3/umps/libumps.e>


void uTLB_RefillHandler();
void exceptionHandler();
void tlbHandler(state_t *interruptedState);
void syscallHandler(state_t *interruptedState);
void trapHandler(state_t *interruptedState);
void passupOrDie();

/**Crea un processo figlio del processo che richiama la syscall e restituisce in v0, -1 in caso di errore, 0 altrimenti.*/

void createProcess(state_t *state);

/**Termina un processo e tutta la progenie*/

void terminateProcess();

/**Verifica se il semaforo esista; se esiste, ne decrementa il valore e se necessario, blocca il pcb processo nella coda del semaforo.
*Se non viene inizialmente trovato un semaforo, ne viene allocato uno nuovo*/

void passeren(state_t *state);

/**Verifica se il semaforo esiste; se esiste, ne incrementa il valore e se necessario, sblocca il pcb in testa alla coda del semaforo.
*Se non viene inizialmente trovato un semaforo, ne incremento comunque il valore. Alla sua allocazione futura avrà quindi un valore diverso da quello di default*/

void verhogen(state_t *state);

/**Mette il processo in attesa sul semaforo di un device passato come parametro*/

void waitForIO(state_t *state);

/**Restituisce il tempo trascorso dal processo chiamante nella cpu*/

void getCpuTime(state_t *state);

/**Mette il processo corrente in attesa sul semaforo dello pseudoclock*/

void waitForClock(state_t *state);

/**Prende dal registro a1, se presente, la supportStruct per il processo: se non è presente ritorna NULL, altrimenti la struttura*/

void getSupportData(state_t *state);

/**Aiuta la terminateProcess() terminando ricorsivamente i suoi figli*/
void sys2Helper(pcb_t* head);

/**Copia lo stato*/
void moveState(state_t *before, state_t *after);

#endif
