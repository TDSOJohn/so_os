#include "../include/vmSupport.h"

swap_t swapPool[POOLSIZE];
int swapPoolSem;

/*TODO : inizializzare area swap pool consigliato 0x20020000*/

void initSwapStructs() {
	int i;
	for (i = 0; i < POOLSIZE; i++) {
		swapPool[i].sw_asid = -1;
	}
	swapPoolSem = 1;
}

void pager() {
	support_t* suppStruct = (support_t *) SYSCALL(8, 0, 0, 0);
	state_t* saved = &(suppStruct->sup_exceptState[0]);
	unsigned int cause = saved->cause;
	cause = cause & 31 << 2;
  	cause = cause >> 2;
	if (cause == 1) {
		/* treat it as a programtrap otherwise continue */
	} else {
		SYSCALL (3, (int) &swapPoolSem, 0,0);
		unsigned int p = saved->entry_hi;
		unsigned int frameNum = getFrame();
		/* if swapPool[frameNum] is occupied, assume it's occupied by logical
		page number k belonging to process x (asid) and that it's dirty */
		if(swapPool[frameNum].sw_asid != -1) {
			unsigned int status = getSTATUS();
			setSTATUS(DISABLEINTS);
			unsigned int procID = swapPool[frameNum].sw_asid;
			unsigned int pageNum = swapPool[frameNum].sw_pageNo;
			swapPool[frameNum].sw_pte->pte_entryLO = swapPool[frameNum].sw_pte->pte_entryLO & 0xFFFFFDFF;
			/* controllare process x page k is currently cached in the tlb */
			uTLB_RefillHandler();
			setSTATUS(status);
			
		}
	}
}

int getFrame() {
	static int frame = 0;
	frame = frame + 1;
	if (frame >= POOLSIZE) {
		frame = 0;
	}
	return (frame);
}