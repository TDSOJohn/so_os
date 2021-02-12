/*********************************P1TEST.C*******************************
 *
 *	Test program for the modules ASL and pcbQueues (phase 1).
 *
 *	Produces progress messages on terminal 0 in addition
 *		to the array ``okbuf[]''
 *		Error messages will also appear on terminal 0 in
 *		addition to the array ``errbuf[]''.
 *
 *		Aborts as soon as an error is detected.
 *
 *      Modified by Michael Goldweber on May 2020
 */

#include "../h/const.h"
#include "../h/types.h"

#include "/usr/local/include/umps3/umps/libumps.h"
#include "../h/pcb.h"
#include "../h/asl.h"


#define MAXPROC	20
#define	MAXSEM	MAXPROC

char okbuf[2048];			/* sequence of progress messages */
char errbuf[128];			/* contains reason for failing */
char msgbuf[128];			/* nonrecoverable error message before shut down */
int sem[MAXSEM];
int onesem;
pcb_t	*procp[MAXPROC], *p, *qa, *q, *firstproc, *lastproc, *midproc;
char *mp = okbuf;


#define TRANSMITTED	5
#define ACK	1
#define PRINTCHR	2
#define CHAROFFSET	8
#define STATUSMASK	0xFF
#define	TERM0ADDR	0x10000254

typedef unsigned int devreg;


void main() {
	int i;

	initPcbs();
	printf("Initialized process control blocks   \n");

	/* Check allocProc */
	for (i = 0; i < MAXPROC; i++) {
		if ((procp[i] = allocPcb()) == NULL)
			printf("allocPcb: unexpected NULL   ");
	}
	if (allocPcb() != NULL) {
		printf("allocPcb: allocated more than MAXPROC entries   ");
	}
	printf("allocPcb ok   \n");

	/* return the last 10 entries back to free list */
	for (i = 10; i < MAXPROC; i++)
		freePcb(procp[i]);
	printf("freed 10 entries   \n");

	/* create a 10-element process queue */
	qa = NULL;
	if (!emptyProcQ(qa)) printf("emptyProcQ: unexpected FALSE   ");
	printf("Inserting...   \n");
	for (i = 0; i < 10; i++) {
		if ((q = allocPcb()) == NULL)
			printf("allocPcb: unexpected NULL while insert   ");
		switch (i) {
		case 0:
			firstproc = q;
			break;
		case 5:
			midproc = q;
			break;
		case 9:
			lastproc = q;
			break;
		default:
			break;
		}
		insertProcQ(&qa, q);
	}
	printf("inserted 10 elements   \n");

	if (emptyProcQ(qa)) printf("emptyProcQ: unexpected TRUE"   );

	/* Check outProc and headProc */
	if (headProcQ(qa) != firstproc)
		printf("headProcQ failed   ");
	q = outProcQ(&qa, firstproc);
	if (q == NULL || q != firstproc)
		printf("outProcQ failed on first entry   ");
	freePcb(q);
	q = outProcQ(&qa, midproc);
	if (q == NULL || q != midproc)
		printf("outProcQ failed on middle entry   ");
	freePcb(q);
	if (outProcQ(&qa, procp[0]) != NULL)
		printf("outProcQ failed on nonexistent entry   ");
	printf("outProcQ ok   \n");

	/* Check if removeProc and insertProc remove in the correct order */
	printf("Removing...   \n");
	for (i = 0; i < 8; i++) {
		if ((q = removeProcQ(&qa)) == NULL)
			printf("removeProcQ: unexpected NULL   ");
		freePcb(q);
	}
	if (q != lastproc)
		printf("removeProcQ: failed on last entry   ");
	if (removeProcQ(&qa) != NULL)
		printf("removeProcQ: removes too many entries   ");

        if (!emptyProcQ(qa))
                printf("emptyProcQ: unexpected FALSE   ");

	printf("insertProcQ, removeProcQ and emptyProcQ ok   \n");
	printf("process queues module ok      \n");

	printf("checking process trees...\n");

	if (!emptyChild(procp[2]))
	  printf("emptyChild: unexpected FALSE   ");

	/* make procp[1] through procp[9] children of procp[0] */
	printf("Inserting...   \n");
	for (i = 1; i < 10; i++) {
		insertChild(procp[0], procp[i]);
	}
	printf("Inserted 9 children   \n");

	if (emptyChild(procp[0]))
	  printf("emptyChild: unexpected TRUE   ");

	/* Check outChild */
	q = outChild(procp[1]);
	if (q == NULL || q != procp[1])
		printf("outChild failed on first child   ");
	q = outChild(procp[4]);
	if (q == NULL || q != procp[4])
		printf("outChild failed on middle child   ");
	if (outChild(procp[0]) != NULL)
		printf("outChild failed on nonexistent child   ");
	printf("outChild ok   \n");

	/* Check removeChild */
	printf("Removing...   \n");
	for (i = 0; i < 7; i++) {
		if ((q = removeChild(procp[0])) == NULL)
			printf("removeChild: unexpected NULL   ");
	}
	if (removeChild(procp[0]) != NULL)
	  printf("removeChild: removes too many children   ");

	if (!emptyChild(procp[0]))
	    printf("emptyChild: unexpected FALSE   ");

	printf("insertChild, removeChild and emptyChild ok   \n");
	printf("process tree module ok      \n");

	for (i = 0; i < 10; i++)
		freePcb(procp[i]);

	printf("So Long and Thanks for All the Fish\n");
}
