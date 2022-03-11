/* See COPYRIGHT for copyright information. */

#include <mmu.h>
#include <error.h>
#include <env.h>
#include <pmap.h>
#include <printf.h>

struct Env *envs = NULL;		// All environments
struct Env *curenv = NULL;	        // the current env

static struct Env_list env_free_list;	// Free list

extern Pde * boot_pgdir;
//static int KERNEL_SP;
extern char * KERNEL_SP;
//
// Calculates the envid for env e.  
//
u_int mkenvid(struct Env *e)
{
	static u_long next_env_id = 0;
	
	// lower bits of envid hold e's position in the envs array
	u_int idx = e - envs;
	
	//printf("env.c:mkenvid:\txx:%x\n",(int)&idx);

	// high bits of envid hold an increasing number
	return(++next_env_id << (1 + LOG2NENV)) | idx;
}

//
// Converts an envid to an env pointer.
//
// RETURNS
//   env pointer -- on success and sets *error = 0
//   NULL -- on failure, and sets *error = the error number
//
int envid2env(u_int envid, struct Env **penv, int checkperm)
{
	struct Env *e;
	    /* Hint: If envid is zero, return curenv.*/
    /*Step 1: Assign value to e using envid. */



    if (e->env_status == ENV_FREE || e->env_id != envid) {
        *penv = 0;
        return -E_BAD_ENV;
    }
    /*     Hint:
     *     Check that the calling env has enough permissions
     *     to manipulate the specified env.
     *     If checkperm is set, the specified env
     *     must be either curenv
     *     or an immediate child of curenv.
     *     If not, error! */
    /*     Step 2: Make a check according to checkperm. */



	*penv = e;
	return 0;
}

int envid2env1(u_int envid, struct Env **penv, int checkperm)
{
	struct Env *e;
	    /* Hint: If envid is zero, return curenv.*/
    /*Step 1: Assign value to e using envid. */

    if (e->env_status == ENV_FREE || e->env_id != envid) {
        *penv = 0;
        return -E_BAD_ENV;
    }
    /*     Hint:
     *     Check that the calling env has enough permissions
     *     to manipulate the specified env.
     *     If checkperm is set, the specified env
     *     must be either curenv
     *     or an immediate child of curenv.
     *     If not, error! */
    /*     Step 2: Make a check according to checkperm. */

	*penv = e;
	//printf("envid2env1(): e:%x\n",e);
	return 0;
}

//
// Marks all environments in 'envs' as free and inserts them into 
// the env_free_list.  Insert in reverse order, so that
// the first call to env_alloc() returns envs[0].
//
void
env_init(void)
{
	int i;

	/*Step 1: Initial env_free_list. */

    /*Step 2: Traverse the elements of 'envs' array,
     * set their status as free and insert them into the env_free_list.
     * Choose the correct loop order to finish the insertion.
     * Make sure, after the insertion, the order of envs in the list
     * should be the same as it in the envs array. */
}

//
// Initializes the kernel virtual memory layout for environment e.
//
// Allocates a page directory and initializes it.  Sets
// e->env_cr3 and e->env_pgdir accordingly.
//
// RETURNS
//   0 -- on sucess
//   <0 -- otherwise 
//
static int
env_setup_vm(struct Env *e)
{
	// Hint:
	int i, r;
	struct Page *p = NULL;

	Pde *pgdir;
	/* Step 1: Allocate a page for the page directory
     * using a function you completed in the lab2 and add its pp_ref.
     * pgdir is the page directory of Env e, assign value for it. */
    if (      ) {
        panic("env_setup_vm - page alloc error\n");
        return r;
    }
    /*Step 2: Zero pgdir's field before UTOP. */


    /*Step 3: Copy kernel's boot_pgdir to pgdir. */

    /* Hint:
     *  The VA space of all envs is identical above UTOP
     *  (except at UVPT, which we've set below).
     *  See ./include/mmu.h for layout.
     *  Can you use boot_pgdir as a template?
     */


    // UVPT maps the env's own page table, with read-only permission.
    e->env_pgdir[PDX(UVPT)]  = e->env_cr3 | PTE_V;
	return 0;
}

//
// Allocates and initializes a new env.
//
// RETURNS
//   0 -- on success, sets *new to point at the new env 
//   <0 -- on failure
//
int
env_alloc(struct Env **new, u_int parent_id)
{
	int r;
	struct Env *e;

	/*Step 1: Get a new Env from env_free_list*/


    /*Step 2: Call certain function(has been completed just now) to init kernel memory layout for this new Env.
     *The function mainly maps the kernel address to this new Env address. */

    /*Step 3: Initialize every field of new Env with appropriate values.*/

    /*Step 4: Focus on initializing the sp register and cp0_status of env_tf field, located at this new Env. */
    e->env_tf.cp0_status = 0x10001004;

    /*Step 5: Remove the new Env from env_free_list. */

}

//
// Sets up the the initial stack and program binary for a user process.
//
// This function loads the complete binary image, including a.out header,
// into the environment's user memory starting at virtual address UTEXT,
// and maps one page for the program's initial stack
// at virtual address USTACKTOP - BY2PG.
// Since the a.out header from the binary is mapped at virtual address UTEXT,
// the actual program text starts at virtual address UTEXT+0x20.
//
// This function does not allocate or clear the bss of the loaded program,
// and all mappings are read/write including those of the text segment.
//
static void
load_icode(struct Env *e, u_char *binary, u_int size)
{
	// Hint: 
	//  Use page_alloc, page_insert, page2kva and e->env_pgdir
	//  You must figure out which permissions you'll need
	//  for the different mappings you create.
	//  Remember that the binary image is an a.out format image,
	//  which contains both text and data.
	struct Page *p=NULL;
	u_long i;
	u_long r;
	unsigned int * index;
	u_char *binary_copy;
	binary+=0x1000;
	
	if ((r = page_alloc(&p)) < 0)
		panic ("page alloc error!");
	p->pp_ref++;
	//cgh:create initial stack for the user process e
	u_long perm;
	perm = PTE_R | PTE_V;
	
	page_insert(e->env_pgdir, p, USTACKTOP - BY2PG, perm);
	
	//load the binary
	for(i = 0 ; i < size; i+=BY2PG)
	{
		if ((r = page_alloc(&p)) < 0)
            panic ("page alloc error!");
		p->pp_ref++;
		binary_copy = page2kva(p);
		//printf("env.c:\tloadicode:\targ1:%x\targ2:%x\n",binary+i, binary_copy);
		bcopy(binary+i, binary_copy, BY2PG);
		page_insert(e->env_pgdir, p, UTEXT+i, perm);
	}

	index=binary;

	//^e->env_tf.pc=UTEXT+(index[6] & 0x000FFFFF);
    e->env_tf.pc=UTEXT;
	//printf("#env.c:\tloadicode():\tthe text is begin @0x%x\tcr3:%x\n",e->env_tf.pc,e->env_cr3);
}

//
// Allocates a new env and loads the a.out binary into it.
//  - new env's parent env id is 0
void
env_create(u_char *binary, int size)
{
	struct Env *e;
}


//
// Frees env e and all memory it uses.
// 
void
env_free(struct Env *e)
{
	Pte *pt;
	u_int pdeno, pteno, pa;

	// Note the environment's demise.
	printf("[%08x] free env %08x\n", curenv ? curenv->env_id : 0, e->env_id);

	// Flush all pages

	for (pdeno = 0; pdeno < PDX(UTOP); pdeno++) {
		if (!(e->env_pgdir[pdeno] & PTE_V))
			continue;
		pa = PTE_ADDR(e->env_pgdir[pdeno]);
		pt = (Pte*)KADDR(pa);
		for (pteno = 0; pteno <= PTX(~0); pteno++)
			if (pt[pteno] & PTE_V)
				page_remove(e->env_pgdir, (pdeno << PDSHIFT) | (pteno << PGSHIFT));
		e->env_pgdir[pdeno] = 0;
		page_decref(pa2page(pa));
	}
	pa = e->env_cr3;
	e->env_pgdir = 0;
	e->env_cr3 = 0;
	page_decref(pa2page(pa));


	e->env_status = ENV_FREE;
	LIST_INSERT_HEAD(&env_free_list, e, env_link);
}

// Frees env e.  And schedules a new env
// if e is the current env.
//
void
env_destroy(struct Env *e) 
{
	env_free(e);
	if (curenv == e) {
		curenv = NULL;
		//panic("bcopy(): src:%x  dst:%x ",(int)KERNEL_SP-sizeof(struct Trapframe),TIMESTACK-sizeof(struct Trapframe));
		bcopy((int)KERNEL_SP-sizeof(struct Trapframe),TIMESTACK-sizeof(struct Trapframe),sizeof(struct Trapframe));
		printf("i am killed ... \n");
		sched_yield();
	}
}


//
// Restores the register values in the Trapframe
//  (does not return)
//
extern void env_pop_tf(struct Trapframe *tf,int id);
extern void lcontext(u_int contxt);

//
// Context switch from curenv to env e.
// Note: if this is the first call to env_run, curenv is NULL.
//  (This function does not return.)
//
void
env_run(struct Env *e)
{
    /*Step 1: save register state of curenv. */
    /* Hint: if there is an environment running, you should do
    *  switch the context and save the registers. You can imitate env_destroy() 's behaviors.*/

    /*Step 2: Set 'curenv' to the new environment. */

    /*Step 3: Use lcontext() to switch to its address space. */

    /*Step 4: Use env_pop_tf() to restore the environment's
     * environment   registers and return to user mode.
     *
     * Hint: You should use GET_ENV_ASID there. Think why?
     * (read <see mips run linux>, page 135-144)
     */
}

