#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"
void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//push eflags
	OPERAND s;
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.eflags.val;
	operand_write(&s);
	//IF=0
	cpu.intr=0;
	cpu.eflags.IF=0;
	//push cs
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.cs.val;
	operand_write(&s);
	//push eip
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.eip;
	operand_write(&s);
	//access to the gate describer
	uint32_t addr=cpu.idtr.base+8*intr_no;
//	printf("idtr:%x,intr_no:%x,addr:%x\n",cpu.idtr.base,intr_no,addr);
	GateDesc gd;
	gd.val[0]=paddr_read(addr,4);
	gd.val[1]=paddr_read(addr+4,4);
//	printf("gd0:%x,gd1:%x\n",gd.val[0],gd.val[1]);
	cpu.cs.val=gd.selector;//cs=selector
	cpu.eip=(gd.offset_31_16<<16)|(gd.offset_15_0);//eip=offset
	load_sreg(SREG_CS);
//	cpu.eip=0xc00300a4;
//	printf("eip:%x\n",cpu.eip);
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
