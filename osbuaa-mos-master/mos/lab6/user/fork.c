// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//

void user_bcopy(const void *src, void *dst, size_t len)
{
	void *max;
	
//	writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%x\n",(int)src,(int)dst,len);
	max = dst + len;
	// copy machine words while possible
	if (((int)src%4==0)&&((int)dst%4==0))
	{
		while (dst + 3 < max)
		{
			*(int *)dst = *(int *)src;
			dst+=4;
			src+=4;
		}
	}
	
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
	//for(;;);
}


void user_bzero(void *v, u_int n)
{
	char *p;
	int m;

	p = v;
	m = n;
	while (--m >= 0)
		*p++ = 0;
}

static void
pgfault(u_int va)
{
	u_int *tmp;
	//	writef("fork.c:pgfault():\t va:%x\n",va);
    
    //map the new page at a temporary place

	//copy the content
	
    //map the page on the appropriate place
	
    //unmap the temporary place
}

//
// Map our virtual page pn (address pn*BY2PG) into the target envid
// at the same virtual address.  if the page is writable or copy-on-write,
// the new mapping must be created copy on write and then our mapping must be
// marked copy on write as well.  (Exercise: why mark ours copy-on-write again if
// it was already copy-on-write?)
// 
static void
duppage(u_int envid, u_int pn)
{
	u_int addr;
	u_int perm;

	//	user_panic("duppage not implemented");
}

//
// User-level fork.  Create a child and then copy our address space
// and page fault handler setup to the child.
//
// Hint: use vpd, vpt, and duppage.
// Hint: remember to fix "env" in the child process!
// 
extern void __asm_pgfault_handler(void);
int
fork(void)
{
	// Your code here.
	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i;


	//The parent installs pgfault using set_pgfault_handler

	//alloc a new alloc


	return newenvid;
}

// Challenge!
int
sfork(void)
{
	user_panic("sfork not implemented");
	return -E_INVAL;
}
