#include "cpu/instr.h"

make_instr_func(call_near) {
	//push eip
	OPERAND s;
	cpu.esp-=data_size/8;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;
	s.val=cpu.eip+1+data_size/8;
	operand_write(&s);

	//jmp near
        OPERAND rel;
	rel.sreg=SREG_DS;
        rel.type = OPR_IMM;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("call", "", 2, &rel);

	cpu.eip += offset;

        return 1 + data_size / 8;
}



make_instr_func(call_near_indirect) {
	//push eip
	OPERAND s;
	cpu.esp-=data_size/8;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=data_size;

	//jmp near indirect
       	 OPERAND d;
	 d.sreg=SREG_CS;
       	d.data_size = data_size;
	uint32_t len= modrm_rm(eip + 1, &d);
        operand_read(&d);
	
	s.val=cpu.eip+1+len;
	operand_write(&s);

	int daddr = sign_ext(d.val, data_size);
	print_asm_1("call", "", 3, &d);

	cpu.eip = daddr;

        return 0;

}

