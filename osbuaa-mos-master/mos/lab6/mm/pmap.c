#include "mmu.h"
#include "pmap.h"
#include "printf.h"
#include "env.h"
#include "error.h"



/* These variables are set by i386_detect_memory() */
u_long maxpa;            /* Maximum physical address */
u_long npage;            /* Amount of memory(in pages) */
u_long basemem;          /* Amount of base memory(in bytes) */
u_long extmem;           /* Amount of extended memory(in bytes) */

Pde* boot_pgdir;

struct Page *pages;
static u_long freemem;

static struct Page_list page_free_list;	/* Free list of physical pages */

void mips_detect_memory()
{
	/* Step 1: Initialize basemem.
     * (When use real computer, CMOS tells us how many kilobytes there are). */

    // Step 2: Calculate corresponding npage value.

	printf("Physical memory: %dK available, ", (int)(maxpa/1024));
	printf("base = %dK, extended = %dK\n", (int)(basemem/1024), (int)(extmem/1024));
}

static void * alloc(u_int n, u_int align, int clear)
{
	extern char end[];

	u_long alloced_mem;
	// initialize freemem if this is the first time
	if (freemem == 0)
		freemem = (u_long)end;

	printf("pmap.c:\talloc from %x",freemem);
	// Your code here:
	//	Step 1: round freemem up to be aligned properly
	freemem = ROUND(freemem, align);
	//	Step 2: save current value of freemem as allocated chunk
	alloced_mem = freemem;	
	//	Step 3: increase freemem to record allocation
	freemem = freemem + n;
	//	Step 4: clear allocated chunk if necessary

	printf(" to %x\n",freemem);

	if (PADDR(freemem) >= maxpa)
	{
		panic("out of memorty\n");
		 return (void *)-E_NO_MEM;
	}
	
	if(clear)	bzero((void *)alloced_mem,n);
	//	Step 5: return allocated chunk
	//panic("pmap.c:\talloc over\n");
	return (void *)alloced_mem;
	
	
}


// 
static Pte* boot_pgdir_walk(Pde *pgdir, u_long va, int create)
{

	Pde *pgdir_entryp;
	Pte *pgtable,*pgtable_entry;
	
	/* Step 1: Get the corresponding page directory entry and page table. */
    /* Hint: Use KADDR and PTE_ADDR to get the page table from page directory
     * entry value. */


    /* Step 2: If the corresponding page table is not exist and parameter `create`
     * is set, create one. And set the correct permission bits for this new page
     * table. */


    /* Step 3: Get the page table entry for `va`, and return it. */

}

//
// Map [va, va+size) of virtual address space to physical [pa, pa+size)
// in the page table rooted at pgdir.  Size is a multiple of BY2PG.
// Use permission bits perm|PTE_P for the entries.
//
void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm)
{
	int i;
	int perm_p;
	/* Step 1: Check if `size` is a multiple of BY2PG. */


    /* Step 2: Map virtual address space to physical address. */
    /* Hint: Use `boot_pgdir_walk` to get the page table entry of virtual address `va`. */

}


void mips_vm_init()
{
	extern char KVPT[];
	extern char end[];
	extern int mCONTEXT;
	extern struct Env *envs;
	struct Env *en;

	Pde* pgdir;
	u_int n;

	pgdir = alloc(BY2PG, BY2PG, 1);
	mCONTEXT=(int)pgdir;

	boot_pgdir = pgdir;

//	printf("pmap.c:\tinit()\tKVPT:%x\tend:%x\n",(int)(pgdir),(int)(&end));

	
//	printf("pmap.c:\tinit()\talloc %d * %d \n",npage,sizeof(struct Page));
	

	pages = (struct Page*)alloc(npage*sizeof(struct Page),BY2PG,1);
	
	n = ROUND(npage * sizeof(struct Page),BY2PG);
	boot_map_segment(pgdir,UPAGES,n,PADDR(pages),PTE_R);




//panic("-------------------init not finish-------------");
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void page_initpp(struct Page *pp);
void
page_init(void)
{
	 /* Step 1: Initialize page_free_list. */
    /* Hint: Use macro `LIST_INIT` defined in include/queue.h. */


    /* Step 2: Align `freemem` up to multiple of BY2PG. */


    /* Step 3: Mark all memory blow `freemem` as used(set `pp_ref`
     * filed to 1) */


    /* Step 4: Mark the other memory as free. */
}


static void
page_initpp(struct Page *pp)
{
	bzero(pp, sizeof(*pp));
}



int
page_alloc(struct Page **pp)
{
// Fill this function in
	struct Page *ppage_temp;
	
	/* Step 1: Get a page from free memory. If fails, return the error code.*/


    /* Step 2: Initialize this page.
     * Hint: use `bzero`. */

}


void
page_free(struct Page *pp)
{
	/* Step 1: If there's still virtual address refers to this page, do nothing. */


    /* Step 2: If the `pp_ref` reaches to 0, mark this page as free and return. */


    /* If the value of `pp_ref` less than 0, some error must occurred before,
     * so PANIC !!! */

	panic("cgh:pp->pp_ref is less than zero\n");
}



void
page_decref(struct Page *pp)
{
	if (--pp->pp_ref == 0)
		page_free(pp);
}


int
pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte)
{
	// Fill this function in
	Pde *pgdir_entryp;
        Pte *pgtable;
	struct Page *ppage;

    /* Step 1: Get the corresponding page directory entry and page table. */


    /* Step 2: If the corresponding page table is not exist(valid) and parameter `create`
     * is set, create one. And set the correct permission bits for this new page
     * table.
     * When creating new page table, maybe out of memory. */


    /* Step 3: Set the page table entry to `*ppte` as return value. */

    return 0;
}





int
page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm) 
{
	// Fill this function in
	u_int PERM;	
	Pte *pgtable_entry;
	PERM = perm | PTE_V;
	
    pgdir_walk(pgdir, va, 0, &pgtable_entry);

	if(pgtable_entry!=0 &&(*pgtable_entry & PTE_V)!=0)
		if(pa2page(*pgtable_entry)!=pp) page_remove(pgdir,va);
		else
		{
			tlb_invalidate(pgdir, va);
			*pgtable_entry = (page2pa(pp)|PERM);
			
			return 0;
		}
    /* Step 2: Update TLB. */

    /* hint: use tlb_invalidate function */


    /* Step 3: Do check, re-get page table entry to validate the insertion. */

    /* Step 3.1 Check if the page can be insert, if can’t return -E_NO_MEM */

    /* Step 3.2 Insert page and increment the pp_ref */
	return 0;
}



struct Page*
page_lookup(Pde *pgdir, u_long va, Pte **ppte)
{
	struct Page *ppage;
	Pte *pte;

    pgdir_walk(pgdir, va, 0, &pte);
//printf("page_lookup:come 1\n");
    if(pte==0) return 0;
    if((*pte & PTE_V)==0) return 0;       //the page is not in memory.
//printf("page_lookup:come 2\n");
    ppage = pa2page(*pte);
	if(ppte) *ppte = pte;
	return ppage;
}




void
page_remove(Pde *pgdir, u_long va) 
{
	// Fill this function in
	Pte *pagetable_entry;
	struct Page *ppage;
//	pgdir_walk(pgdir, va, 0, &pagetable_entry);
	
//	if(pagetable_entry==0) return;
//	if((*pagetable_entry & PTE_P)==0) return;	//the page is not in memory.
	
//	ppage = pa2page(*pagetable_entry);
	ppage = page_lookup(pgdir, va, &pagetable_entry);
	if (ppage == 0) return;
	ppage->pp_ref--;
	if(ppage->pp_ref==0) page_free(ppage);
	*pagetable_entry=0;
//printf("	in page_remove pagetalbe_entry=%x\n",pagetable_entry);
	tlb_invalidate(pgdir, va);
	return;
}


//TODO
void
tlb_invalidate(Pde *pgdir, u_long va)
{
	//printf("tlb_invalidate():\tva=%x\tcurenv:%x\n",va,(int)curenv);

	//if (va==0x60000000) while(1);

	if (curenv)
		tlb_out(PTE_ADDR(va)|GET_ENV_ASID(curenv->env_id));
	else
		tlb_out(PTE_ADDR(va));

}



void
page_check(void)
{
	struct Page *pp, *pp0, *pp1, *pp2;
	struct Page_list fl;

	// should be able to allocate three pages
	pp0 = pp1 = pp2 = 0;
	assert(page_alloc(&pp0) == 0);
	assert(page_alloc(&pp1) == 0);
	assert(page_alloc(&pp2) == 0);

	assert(pp0);
	assert(pp1 && pp1 != pp0);
	assert(pp2 && pp2 != pp1 && pp2 != pp0);

	// temporarily steal the rest of the free pages
	fl = page_free_list;
	LIST_INIT(&page_free_list);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// there is no free memory, so we can't allocate a page table 
	assert(page_insert(boot_pgdir, pp1, 0x0, 0) < 0);

	// free pp0 and try again: pp0 should be used for page table
	page_free(pp0);
	assert(page_insert(boot_pgdir, pp1, 0x0, 0) == 0);
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));
	assert(va2pa(boot_pgdir, 0x0) == page2pa(pp1));
	assert(pp1->pp_ref == 1);

	// should be able to map pp2 at BY2PG because pp0 is already allocated for page table
	assert(page_insert(boot_pgdir, pp2, BY2PG, 0) == 0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp2));
	assert(pp2->pp_ref == 1);
	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);
//printf("why\n");
	// should be able to map pp2 at BY2PG because it's already there
	assert(page_insert(boot_pgdir, pp2, BY2PG, 0) == 0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp2));
	assert(pp2->pp_ref == 1);
//printf("It is so unbelivable\n");
	// pp2 should NOT be on the free list
	// could happen in ref counts are handled sloppily in page_insert
	assert(page_alloc(&pp) == -E_NO_MEM);

	// should not be able to map at PDMAP because need free page for page table
	assert(page_insert(boot_pgdir, pp0, PDMAP, 0) < 0);

	// insert pp1 at BY2PG (replacing pp2)
	assert(page_insert(boot_pgdir, pp1, BY2PG, 0) == 0);

	// should have pp1 at both 0 and BY2PG, pp2 nowhere, ...
	assert(va2pa(boot_pgdir, 0x0) == page2pa(pp1));
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp1));
	// ... and ref counts should reflect this
	assert(pp1->pp_ref == 2);
	assert(pp2->pp_ref == 0);

	// pp2 should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp2);

	// unmapping pp1 at 0 should keep pp1 at BY2PG
	page_remove(boot_pgdir, 0x0);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, BY2PG) == page2pa(pp1));
	assert(pp1->pp_ref == 1);
	assert(pp2->pp_ref == 0);

	// unmapping pp1 at BY2PG should free it
	page_remove(boot_pgdir, BY2PG);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, BY2PG) == ~0);
	assert(pp1->pp_ref == 0);
	assert(pp2->pp_ref == 0);

	// so it should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp1);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// forcibly take pp0 back
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));
	boot_pgdir[0] = 0;
	assert(pp0->pp_ref == 1);
	pp0->pp_ref = 0;

	// give free list back
	page_free_list = fl;

	// free the pages we took
	page_free(pp0);
	page_free(pp1);
	page_free(pp2);

	printf("page_check() succeeded!\n");
}



void pageout(int va,int context)
{
	u_long r;
	struct Page *p=NULL;

	if (context<0x80000000)
		panic("tlb refill and alloc error!");
	if ((va>0x7f400000)&&(va<0x7f800000))
		panic(">>>>>>>>>>>>>>>>>>>>>>it's env's zone");

	if (va<0x10000)
		panic("^^^^^^TOO LOW^^^^^^^^^"); 
	if ((r = page_alloc(&p)) < 0)
		panic ("page alloc error!");
	p->pp_ref++;

	page_insert((int*)context, p, VA2PFN(va), PTE_R);
	printf("pageout:\t@@@___0x%x___@@@  ins a page \n",va);
}

