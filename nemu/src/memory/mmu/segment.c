#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */

	return offset+cpu.segReg[sreg].base;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	//printf("sreg:%x\n",sreg);
	uint32_t index=cpu.segReg[sreg].index;
	uint32_t laddr=cpu.gdtr.base+sizeof(SegDesc)*index;	
	SegDesc sd;
	sd.val[0]=laddr_read(laddr,4);
	sd.val[1]=laddr_read(laddr+4,4);
	cpu.segReg[sreg].base=(sd.base_31_24<<24)|(sd.base_23_16<<16)|(sd.base_15_0);
	cpu.segReg[sreg].limit=(sd.limit_19_16<<16)|(sd.limit_15_0);
	
	//printf("limit:%x,base:%x/n",cpu.segReg[sreg].limit,cpu.segReg[sreg].base);
	assert(cpu.segReg[sreg].limit==0xfffff);
	assert(cpu.segReg[sreg].base==0);
}
