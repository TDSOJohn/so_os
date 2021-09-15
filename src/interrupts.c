#include "../include/interrupts.h"

extern cpu_t initialTOD;
extern cpu_t currentTOD;

unsigned int findLine(unsigned int cause) {
  if ((cause & 0x100) != 0) return 0;
  else if ((cause & 0x200) != 0) return 1;
  else if ((cause & 0x400) != 0) return 2;
  else if ((cause & 0x800) != 0) return 3;
  else if ((cause & 0x1000) != 0) return 4;
  else if ((cause & 0x2000) != 0) return 5;
  else if ((cause & 0x4000) != 0) return 6;
  else if ((cause & 0x8000) != 0) return 7;

  return -1;
}

unsigned int findDevice(unsigned int line) {
  unsigned int *start = (unsigned int *)(0x10000040 + 4*(line-3));
  unsigned int startu = *start;
  if ((startu & 0x1) != 0) return 0;
  else if ((startu & 0x2) != 0) return 1;
  else if ((startu & 0x4) != 0) return 2;
  else if ((startu & 0x8) != 0) return 3;
  else if ((startu & 0x10) != 0) return 4;
  else if ((startu & 0x20) != 0) return 5;
  else if ((startu & 0x40) != 0) return 6;
  else if ((startu & 0x80) != 0) return 7;

  return -1;
}

void interruptHandler(state_t *interruptedState) {

    unsigned int line = findLine(interruptedState->cause);
    pcb_t* p;
    if (line == 0) {

    }
    else if (line == 1) {
      if (currentProcess != NULL ) {
        setTIMER(5000);
        unsigned int currentTOD, newTOD;
        STCK(currentTOD);
        newTOD = currentTOD - initialTOD;
        initialTOD = currentTOD;
        currentProcess->p_time += newTOD;
        moveState(interruptedState, &(currentProcess->p_s));
        insertProcQ(&readyQueue,currentProcess);
        scheduler();
      }
    }
    else if (line == 2) {
      LDIT (100*1000);
      p = removeBlocked(&(deviceSem[49-1]));
      while (p != NULL) {
        insertProcQ(&readyQueue, p);
        softBlockCount = softBlockCount -1;
        p = removeBlocked(&(deviceSem[49-1]));
      }
      deviceSem[49-1] = 0;
      if (currentProcess != NULL) LDST(interruptedState);
      else scheduler();

    } else if (line <= 7 ) {
      unsigned int devNum = findDevice(line);
      if (line >= 3 && line <= 6) {
        int *sem = &deviceSem[8 * (line - 3) + devNum];
        dtpreg_t *devReg = (dtpreg_t*)(0x10000054 + ((line - 3) * 0x80) +(devNum * 0x10));
        unsigned int statusCode = devReg->status;
        devReg->command = ACK;
        *sem = *sem + 1;
        if (*sem <= 0) {
          p = removeBlocked(sem);
          if (p != NULL) {
            p->p_s.reg_v0 = statusCode;
            insertProcQ(&readyQueue, p);
            softBlockCount = softBlockCount -1;
          }
        }

      } else if (line == 7) {
        termreg_t *termReg = (termreg_t*)(0x10000054 + ((line - 3) * 0x80) + (devNum * 0x10));
        if ((termReg->recv_status&0xFF) == 5) {

        }
        if ((termReg->transm_status&0xFF) == 5) {
          int *sem = &deviceSem[8 * (line - 3) + devNum];
          unsigned int statusCode = termReg->transm_status;
          termReg->transm_command = ACK;
          *sem = *sem +1;
          if (*sem <= 0) {
            p = removeBlocked(sem);
            if (p != NULL) {
              p->p_s.reg_v0 = statusCode;
              insertProcQ(&readyQueue, p);
              softBlockCount = softBlockCount -1;
            }
          }
        }
      }
      if (currentProcess != NULL) {
        LDST(interruptedState);
      } else {
        scheduler();
      }
    }

}
