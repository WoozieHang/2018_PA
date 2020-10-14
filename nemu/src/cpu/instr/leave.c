#include "cpu/instr.h"

make_instr_func(leave) {
	cpu.esp=cpu.ebp;
	//pop ebp
	OPERAND s;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;
	operand_read(&s);
	print_asm_0("leave", "", 1);
	cpu.ebp=s.val;
	cpu.esp+=data_size/8;

        return 1;
}
