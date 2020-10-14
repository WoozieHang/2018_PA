#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
typedef union{
	struct{
		uint32_t pf_off :12;
		uint32_t pt_idx	:10;
		uint32_t pdir_idx: 10;
	
	};
	uint32_t addr;

}Page_Addr;

paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	Page_Addr *addr=(void*)&laddr;
	paddr_t pdir_base=cpu.cr3.val & ~PAGE_MASK;
	
	PDE pde;
	pde.val=paddr_read(pdir_base+addr->pdir_idx*4,4);
	assert(pde.present);
	
	paddr_t pt_base=pde.val & ~PAGE_MASK;
	PTE pte;
	pte.val=paddr_read(pt_base+addr->pt_idx*4,4);
	assert(pte.present);
	
	paddr_t p_base=pte.val& ~PAGE_MASK;
	return p_base|(laddr&PAGE_MASK);
#else
	//uint32_t paddr=tlb_read(laddr)|(laddr&PAGE_MASK);
	//printf("\nfor debug!tlb_p_addr:%x\n",paddr);	
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
