#include "../include/scheduler.h"

cpu_t initialTOD;
cpu_t currentTOD;

void scheduler() {

    if(currentProcess != NULL) {
      STCK(currentTOD);
      currentProcess->p_time = currentProcess->p_time + (currentTOD - initialTOD);
    }

    if (!emptyProcQ(readyQueue)) {
      STCK(initialTOD);
      currentProcess = removeProcQ(&readyQueue);
      setTIMER(5000);
      LDST(&(currentProcess->p_s));
    } else {

      if (processCount == 0) {
        HALT();
      }
      if (processCount > 0 && softBlockCount == 0 ) {
        PANIC();
      }
      if ( processCount > 0 && softBlockCount > 0){
        currentProcess = NULL;
        setTIMER(1000000000);
        unsigned int status = getSTATUS();
        status = status | IECON | IMON;
        setSTATUS(status);
        WAIT();
      }
    }
}
