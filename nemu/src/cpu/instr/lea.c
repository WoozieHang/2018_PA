#include "cpu/instr.h"

make_instr_func(lea) {
	OPERAND rm, r;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	r.val = rm.addr;
	operand_write(&r);
	print_asm_2("lea", "", 10, &rm,&r);

	return len;
}
