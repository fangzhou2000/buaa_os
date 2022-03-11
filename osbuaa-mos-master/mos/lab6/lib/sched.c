#include <env.h>
#include <pmap.h>
#include <printf.h>

// Trivial temporary clock interrupt handler,
// called from clock_interrupt in locore.S
/*void
clock(void)
{
	printf("*");
}*/


// The real clock interrupt handler,
// implementing round-robin scheduling
void
sched_yield(void)
{
static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
}

