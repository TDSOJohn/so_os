#ifndef PANDOS_TYPES_H_INCLUDED
#define PANDOS_TYPES_H_INCLUDED

/****************************************************************************
 *
 * This header file contains utility types definitions.
 *
 ****************************************************************************/

#include <umps3/umps/types.h>
#include "pandos_const.h"

typedef signed int cpu_t;
typedef unsigned int memaddr;

/* process table entry type */
typedef struct pcb_t
{
	/* process queue fields */
    struct pcb_t   *p_next;							/* ptr to next entry			*/
    struct pcb_t   *p_prev; 						/* ptr to previous entry		*/

	/* process tree fields */
	struct pcb_t	*p_prnt, 						/* ptr to parent				*/
					*p_child,						/* ptr to 1st child				*/
					*p_next_sib,					/* ptr to next sibling 			*/
					*p_prev_sib;					/* ptr to prev. sibling			*/

	/* process status information */
/*	state_t     p_s; */        /* processor state        */

	/* add more fields here */
    int             *p_semADD;
}  pcb_t, *pcb_PTR;

/*  definizione struttura del semaforo */
typedef struct semd_t
{
	struct semd_t		*s_next; 	/* prossimo elemento della ASL */
	int 				*s_semAdd; 	/* puntatore al semaforo */
	pcb_t 				*s_procQ; 	/* puntatore di coda ad una process queue */
} semd_t;


#endif
