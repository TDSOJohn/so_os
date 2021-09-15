#include "../include/exceptions.h"

extern cpu_t initialTOD;
extern cpu_t currentTOD;



void moveState(state_t *before, state_t *after) {

    after->cause = before->cause;
    after->entry_hi = before->entry_hi;
    after->pc_epc = before->pc_epc;
    after->status = before->status;
    int i;
    for(i=0; i < STATE_GPR_LEN; i++) {
        after->gpr[i] = before->gpr[i];
    }
    after->hi = before->hi;
    after->lo = before->lo;
}

void uTLB_RefillHandler () {
  state_t *saved = (state_t *) 0x0FFFF000;
  unsigned int p = saved->entry_hi;
  int n = ((p & 0xFFFFF000) >> VPNSHIFT) - 0x80000;
  if (p > 30 || p < 0) {
    p = 31;
  }
	setENTRYHI(currentProcess->p_supportStruct->sup_privatePgTbl[n].pte_entryHI);
  setENTRYLO(currentProcess->p_supportStruct->sup_privatePgTbl[n].pte_entryLO);
	TLBWR();

	LDST ((state_t *) 0x0FFFF000);
}

void passupOrDie(state_t* state, int num) {
    if(currentProcess->p_supportStruct == NULL){
        terminateProcess();
      } else {
        moveState(state, &(currentProcess->p_supportStruct->sup_exceptState[num]));
        LDCXT(currentProcess->p_supportStruct->sup_exceptContext[num].stackPtr,currentProcess->p_supportStruct->sup_exceptContext[num].status,
              currentProcess->p_supportStruct->sup_exceptContext[num].pc);
      }

}


void trapHandler(state_t *interruptedState) {
  passupOrDie(interruptedState, GENERALEXCEPT);
}


void tlbHandler(state_t *interruptedState) {
  passupOrDie(interruptedState, PGFAULTEXCEPT);
}

void exceptionHandler() {
  state_t *interruptedState = (state_t*) 0x0FFFF000;
  unsigned int cause = interruptedState->cause;
  cause = cause & 31 << 2;
  cause = cause >> 2;
  if (cause == 0) {
    interruptHandler(interruptedState);
  } else if(cause >= 1 && cause <= 3) {
    tlbHandler(interruptedState);
  } else if (cause == 8) {
    syscallHandler(interruptedState);
  } else if ((cause >= 4 && cause <= 7) || (cause >= 9 && cause <= 12)){
    trapHandler(interruptedState);
  }

}


void syscallHandler(state_t *interruptedState) {
  unsigned int kernelMode = interruptedState->status;
  kernelMode = (kernelMode & 0x8) >> 0x3;
  if (kernelMode == 0) {
    switch (interruptedState->reg_a0) {
      case CREATEPROCESS :
        createProcess(interruptedState);
      break;
      case TERMPROCESS :
        terminateProcess();
      break;
      case PASSEREN :
        passeren(interruptedState);
      break;
      case VERHOGEN :
        verhogen(interruptedState);
      break;
      case IOWAIT:
        waitForIO(interruptedState);
      break;
      case GETTIME:
        getCpuTime(interruptedState);
      break;
      case CLOCKWAIT:
        waitForClock(interruptedState);
      break;
      case GETSUPPORTPTR :
        getSupportData(interruptedState);
      break;
      default :
        passupOrDie(interruptedState, GENERALEXCEPT);
      break;
    }
  } else {
    interruptedState->cause = interruptedState->cause | (10 << 2);
    trapHandler(interruptedState);
  }
}




void createProcess(state_t *state) {
  pcb_t* son = allocPcb();
  if (son == NULL) {
    state->reg_v0 = -1;
  } else {
    moveState((state_t*) state->reg_a1, &(son->p_s));
    son->p_time = 0;
    son->p_semAdd = NULL;
    son->p_supportStruct = (support_t*) state->reg_a2;
    insertProcQ(&readyQueue, son);
    insertChild(currentProcess, son);
    processCount = processCount + 1;
  }
  state->reg_v0 = 0;
  state->pc_epc += 4;
  moveState(state, &(currentProcess->p_s));
  LDST(state);
}


void terminateProcess() {

  if(emptyChild(currentProcess)){
    outChild(currentProcess);
    freePcb(currentProcess);
    --processCount;
	} else {
    		sys2Helper(currentProcess);


	}
	/* no current process anymore */
	currentProcess = NULL;
	/* call scheduler */
	scheduler();
}

void sys2Helper(pcb_t* head){
	/* remove all children */
	while(!emptyChild(head)){
		/* nuke it till it pukes */
		sys2Helper(removeChild(head));
	}



	if(head -> p_semAdd != NULL){
		/* process blocked, remove self from ASL */
		int* sem = head -> p_semAdd;


		 head = outBlocked(head);


		/* check if blocked on device */
		if(sem >= &(deviceSem[0]) && sem <= &(deviceSem[49-1])){

			softBlockCount = softBlockCount - 1;
		} else {
			(*sem) = (*sem) + 1; /* increment semaphore */
		}


	} else if (head == currentProcess) {
		/* remove process from it's parent */
		outChild(currentProcess);
	} else {
		/* remove process from readyQueue */
		outProcQ(&readyQueue, head);
	}
	/* free self after we have no more children */
	freePcb(head);
	processCount = processCount -1;
}



void passeren(state_t *state) {
  int *semP = (int*) state->reg_a1;
  *semP = *semP - 1;
  if (*semP < 0) {
    moveState(state, &(currentProcess->p_s));
    currentProcess->p_s.pc_epc += 4;
    unsigned int currentTOD, newTOD;
    STCK(currentTOD);
    newTOD = currentTOD - initialTOD;
    initialTOD = currentTOD;
    currentProcess->p_time += newTOD;
    /*/aggiornare il tempo accumulato dal processo*/
    if(insertBlocked(semP, currentProcess)) {

  }
  else {
      currentProcess = NULL;
      softBlockCount++;
      scheduler();
    }
}
  state->pc_epc += 4;
    moveState(state, &(currentProcess->p_s));
    LDST(state);
  }


void verhogen(state_t *state) {
  int *semV = (int*) state -> reg_a1;
  *semV = *semV + 1;
  if (*semV <= 0) {
    pcb_t *process = removeBlocked(semV);
    if (process != NULL) {
      insertProcQ(&readyQueue,process);
      softBlockCount = softBlockCount - 1;
      process->p_semAdd = NULL;

    }
  }   state->pc_epc += 4;
      moveState(state, &(currentProcess->p_s));
      LDST(state);

}

void waitForIO(state_t *state) {
  int lineNumber, devNumber, read;
  lineNumber = state->reg_a1;
  devNumber = state->reg_a2;
  read = state->reg_a3;
  if ((lineNumber < 3 || lineNumber > 7) || (devNumber < 0 || devNumber > 7)) {

  } else {
    if(lineNumber == 7 && read) {
      lineNumber++;
    }
    deviceSem[8 * (lineNumber -3 ) + devNumber] = deviceSem[8 * (lineNumber -3 ) + devNumber] -1;
    moveState(state, &(currentProcess->p_s));
    currentProcess->p_s.pc_epc += 4;
    unsigned int currentTOD, newTOD;
    STCK(currentTOD);
    newTOD = currentTOD - initialTOD;
    initialTOD = currentTOD;
    currentProcess->p_time += newTOD;
    /*/aggiornare il tempo accumulato dal processo*/
  if(insertBlocked(&(deviceSem[8 * (lineNumber -3 ) + devNumber]), currentProcess)) {
    /*/insertBlocked ha fallito*/
  }
  else {
      currentProcess = NULL;
      softBlockCount++;
      scheduler();
    }
  }

		currentProcess->p_s.reg_v0 = deviceSem[8 *(lineNumber - 3) +devNumber];
}

void getCpuTime(state_t *state) {
  unsigned int currentTOD, newTOD;
  STCK(currentTOD);
  newTOD = currentTOD - initialTOD;
  initialTOD = currentTOD;
  currentProcess->p_time += newTOD;
  state->reg_v0 = currentProcess->p_time;
  state->pc_epc += 4;
  moveState(state, &(currentProcess->p_s));
  LDST(state);

}

void waitForClock(state_t *state) {
  deviceSem[49-1] = deviceSem[49-1] - 1;
  moveState(state, &(currentProcess->p_s));
  currentProcess->p_s.pc_epc += 4;
  unsigned int currentTOD, newTOD;
  STCK(currentTOD);
  newTOD = currentTOD - initialTOD;
  initialTOD = currentTOD;
  currentProcess->p_time += newTOD;
  /*/aggiornare il tempo accumulato dal processo*/
if(insertBlocked(&deviceSem[49-1], currentProcess)) {
  /*/insertBlocked ha fallito*/
}
else {
    currentProcess = NULL;
    softBlockCount++;
    scheduler();
  }

}

void getSupportData(state_t *state) {
  state->reg_v0 = (unsigned int) currentProcess->p_supportStruct;
  state->pc_epc += 4;
  moveState(state, &(currentProcess->p_s));
  LDST(state);
}
