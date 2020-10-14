#include"cpu/instr.h"
static void instr_execute_1op(){
	OPERAND s;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;
	operand_read(&s);
	opr_src.val=s.val;
	operand_write(&opr_src);
	cpu.esp+=data_size/8;
}

make_instr_impl_1op(pop,r,v)

make_instr_func(popa) {
	//pop edi
	OPERAND s;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.edi=s.val;
	cpu.esp+=4;
	//pop esi
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.esi=s.val;
	cpu.esp+=4;

	//pop ebp
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.ebp=s.val;
	cpu.esp+=4;

	//pop throwaway
	cpu.esp+=4;

	//pop ebx
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.ebx=s.val;
	cpu.esp+=4;

	//pop edx
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.edx=s.val;
	cpu.esp+=4;

	//pop ecx
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.ecx=s.val;
	cpu.esp+=4;

	//pop eax
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.eax=s.val;
	cpu.esp+=4;
	
	return 1;
}
