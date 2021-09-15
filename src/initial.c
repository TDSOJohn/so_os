#include "../include/initial.h"

int processCount;
int softBlockCount;
pcb_t* readyQueue;
pcb_t* currentProcess;
int deviceSem[49];

extern void test();
extern void uTLB_RefillHandler();
extern void exceptionHandler();

int main () {

  passupvector_t *pUV = (passupvector_t*) 0x0FFFF900;
  pUV->tlb_refill_handler = (memaddr ) uTLB_RefillHandler;
  pUV->tlb_refill_stackPtr = 0x20001000;
  pUV->exception_handler = (memaddr) exceptionHandler;
  pUV->exception_stackPtr = 0x20001000;

  initPcbs();
  initASL();

  processCount = 0;
  softBlockCount = 0;
  readyQueue = mkEmptyProcQ();
  currentProcess = NULL;


  int i;
  
  for (i = 0; i < 49; i++) {
    deviceSem[i] = 0;

  }

  LDIT(100*1000);
  pcb_t *p = allocPcb();
  unsigned int status = 0;
  status = status | IEPON | IMON | TEBITON;
  p->p_s.status = status;
  RAMTOP(p->p_s.reg_sp);
  p->p_s.pc_epc = (memaddr) test;
  p->p_s.reg_t9 = (memaddr) test;
  p->p_time = 0;
  p->p_semAdd = NULL;
  p->p_supportStruct = NULL;
  insertProcQ(&(readyQueue), p);
  processCount = processCount + 1;
  scheduler();

  return -1;
}
