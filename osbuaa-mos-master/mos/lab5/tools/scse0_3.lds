OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
  . = 0x80010000;

  /*** exercise 3.12 ***/

  .text : {
        *(.text)
	*(.rodata)
	*(.rodata1)
	*(.rodata.str1.4)
        }

  .reginfo : { *(.reginfo) }

  .date : {
	*(.data)
	}

  .bss  : {
	*(.dynbss)
	*(.bss)
  }
   
  . = 0x803F8000;
  .kernelstk : {
	*(.data.stk)
	}

   . = 0x80400000;
   KVPT = . ;
.kernelvpt : {
  *(.data.vpt)
    }

   end = . ;
}
