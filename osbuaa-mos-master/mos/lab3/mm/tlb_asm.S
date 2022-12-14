#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

LEAF(tlb_out)
//1: j 1b
nop
	mfc0	k1,CP0_ENTRYHI
	mtc0	a0,CP0_ENTRYHI
	nop
	// insert tlbp or tlbwi
	nop
	nop
	nop
	nop
	mfc0	k0,CP0_INDEX
	bltz	k0,NOFOUND
	nop
		mtc0	zero,CP0_ENTRYHI
		mtc0	zero,CP0_ENTRYLO0
		nop
	// insert tlbp or tlbwi
	//add	k0, 40
	//sb	k0, 0x90000000
	//li	k0, '>'
	//sb	k0, 0x90000000
NOFOUND:

	mtc0	k1,CP0_ENTRYHI
	
	j	ra
	nop
END(tlb_out)
