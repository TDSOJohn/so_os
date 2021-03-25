#include "../h/const.h"
#include "../h/types.h"
/*#include "/usr/local/include/umps3/umps/libumps.e"*/

int processCount;
int softBlockCount;
pcb_t *readyQueue;
pcb_t *currentProcess;

/*Semafori*/

int semDisk[8];
int semTape[8];
int semEthernet[8];
int semPrinter[8];
int semTerminalRead[8];
int semTerminalWrite[8];
int pseudoClock;
passupvector_t* passupvector;

void uTLB_RefillHandler() {
	setENTRYHI(0x80000000);
	setENTRYLO(0x00000000);
	TLBWR();
	LDST ((state_PTR) 0x0FFFF000);
}

void funzioneBella()
{

}

int main () {
	initPcbs();
	initSemd();

	*((memaddr*) 0x0FFFF900) = (memaddr) uTLB_RefillHandler;
	*((memaddr*) 0x0FFFF900 + 0x04) = (memaddr) 0x2000.1000;
	*((memaddr*) 0x0FFFF900 + 0x08) = (memaddr) funzioneBella;

	*((memaddr*) 0x0FFFF900 + 0x0C) = (memaddr) 0x2000.1000;



	processCount = 0;
	softBlockCount = 0;
	readyQueue = mkEmptyProcQ();
	currentProcess = NULL;

	for(i=0;i<8;i++){
		semDisk[i] = 0;
		semTape[i] = 0;
		semEthernet[i] = 0;
		semPrinter[i] = 0;
		semTerminalRead[i] = 0;
		semTerminalWrite[i] = 0;
	}
	pseudoClock = 0;

	init();

	PANIC();
	return -1;
}
