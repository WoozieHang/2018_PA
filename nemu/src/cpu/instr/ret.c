#include "cpu/instr.h"


make_instr_func(iret){
	OPERAND s;
	//pop eip
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.eip=s.val;
	cpu.esp+=4;

	//pop cs
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.cs.val=s.val;
	cpu.esp+=4;

	//pop eflags
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	operand_read(&s);
	cpu.eflags.val=s.val;
	cpu.esp+=4;


	return 0;
}


make_instr_func(ret_near) {
	//pop eip
	OPERAND s;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;
	operand_read(&s);
	print_asm_0("ret", "", 1);
	cpu.eip=s.val;
	cpu.esp+=data_size/8;

        return 0;
}


make_instr_func(ret_near_imm16){

//pop eip
	OPERAND s;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;
	s.sreg=SREG_DS;
	operand_read(&s);
	print_asm_0("ret", "", 1);
	cpu.eip=s.val;
	cpu.esp+=data_size/8;
	operand_read(&opr_src);
	cpu.esp+=opr_src.val;
        return 0;

}

