#include "cpu/instr.h"

make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 3, &rel);

	cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_far_imm){
	OPERAND d;
	d.data_size=32;
	d.sreg=SREG_CS;
	d.type=OPR_IMM;
	d.addr=eip+1;

	operand_read(&d);
	uint32_t pc=d.val;

	d.data_size=16;
	d.addr=eip+5;
	operand_read(&d);
	cpu.cs.val=d.val;
	//printf("d.addr:%x",d.addr);
	load_sreg(1);
	//printf("d.val:%x,cpu.cs.val:%x\n",d.val,cpu.cs.val);
	cpu.eip=pc;
	print_asm_1("jmp", "", 3, &d);

	return 0;

}

make_instr_func(jmp_short) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, 8);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 2;
}



make_instr_func(jmp_near_indirect) {


        OPERAND d;
       	d.data_size = data_size;
	d.sreg=SREG_DS;
	 modrm_rm(eip + 1, &d);
        operand_read(&d);

	int daddr = sign_ext(d.val, data_size);
	print_asm_1("jmp", "", 3, &d);

	cpu.eip = daddr;

        return 0;
}

