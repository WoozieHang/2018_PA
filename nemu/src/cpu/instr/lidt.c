#include"cpu/instr.h"
make_instr_func(lidt){
	int len=1;
	OPERAND d;
	d.data_size=16;
	d.type=OPR_IMM;
	len+=modrm_rm(eip+1,&d);
	d.sreg=SREG_DS;
	operand_read(&d);
	cpu.idtr.limit=d.val;
	d.data_size=32;
	d.addr+=2;
	operand_read(&d);
	cpu.idtr.base=d.val;
	print_asm_1("lidt","",5,&d);
	return len;
}
