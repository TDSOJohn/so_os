#ifndef interrupts_h
#define interrupts_h


#include "pandos_const.h"
#include "pandos_types.h"
#include "../include/pcb.h"
#include "../include/asl.h"
#include "../include/scheduler.h"
#include "../include/initial.h"
#include "../include/exceptions.h"
#include <umps3/umps/libumps.e>

/**Handler per gli interrupt. L'indirizzo di questa funzione è salvato nella ROM Reserved Frame come gestore degli interrupt.*/

void interruptHandler(state_t *interruptedState);

/**Trova il device dai cui e' stato sollevato l'interrupt*/

unsigned int findDevice(unsigned int line);

/**Trova la linea degli interrupt. A linee piu' basse sono associate priorita' piu' alte*/

unsigned int findLine(unsigned int cause);

#endif
