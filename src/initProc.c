#include "../include/initProc.h"

static support_t suppStruct[UPROCMAX+1];


int mainSem;
int mutexSemArray[49];

void initSuppStructs (int id, memaddr topStack) {
	suppStruct[id].sup_asid = id;
	/*suppStruct[i].sup_exceptContext[1].pc = (memaddr ) generalException;*/
	suppStruct[id].sup_exceptContext[1].status =  TEBITON | IEPON | IMON;
	suppStruct[id].sup_exceptContext[1].stackPtr = (memaddr ) topStack;
	suppStruct[id].sup_exceptContext[0].pc = (memaddr) pager;
	suppStruct[id].sup_exceptContext[0].status =  TEBITON | IEPON | IMON;
	suppStruct[id].sup_exceptContext[0].stackPtr = (memaddr) (topStack + PAGESIZE);
	for (int i = 0; i < MAXPAGES - 1; i++ ) {
		suppStruct[id].sup_privatePgTbl[i].pte_entryHI = 0x80000000 + (i << VPNSHIFT) + (id << ASIDSHIFT);
		suppStruct[id].sup_privatePgTbl[i].pte_entryLO = DIRTYON;
	}

	suppStruct[id].sup_privatePgTbl[MAXPAGES - 1].pte_entryHI = 0xBFFFF000 + (id << ASIDSHIFT);
	suppStruct[id].sup_privatePgTbl[MAXPAGES - 1].pte_entryLO = DIRTYON;
	

}

void initUserProc(int id) {
	memaddr ramTOP;
	RAMTOP(ramTOP);
	memaddr topStack = ramTOP - (2*id*PAGESIZE);
	state_t newState;
	newState.entry_hi = id << ASIDSHIFT;
	newState.pc_epc = newState.reg_t9 = UPROCSTARTADDR;
	newState.reg_sp = 0xC0000000;
	newState.status = IMON | IEPON | TEBITON | USERPON;

	initSuppStructs(id, topStack);
	int status = SYSCALL (CREATEPROCESS, (int) &newState, (int) &(suppStruct[id]), 0);
	if (status != OK) {
		SYSCALL(TERMPROCESS, 0 , 0, 0);
	}

}



void testo() {
	initSwapStructs();
	for (int i = 0; i < 49; i++) {
		mutexSemArray[i] = 1;
	}
	for (int id = 1; id <= UPROCMAX; id++) {
		initUserProc(id);
	}
	mainSem = 0;

	for(int i=0; i < UPROCMAX; i++)
    {
		SYSCALL(PASSEREN, (int) &mainSem, 0, 0);
	}
    
    /*dopo questo HALT*/
	SYSCALL(TERMPROCESS, 0, 0, 0);
}

	
	

