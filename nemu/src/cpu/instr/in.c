#include "cpu/instr.h"

make_instr_func(in_b) {
	uint16_t port=cpu.gpr[2]._16;
	cpu.gpr[0]._8[0]=pio_read(port,1);
	return 1;
}

make_instr_func(in_v) {
//	int x;
//	scanf("%d",&x);
uint16_t port=cpu.gpr[2]._16;
	if(data_size==16)
		cpu.gpr[0]._16=pio_read(port,2);

	else if(data_size==32)
		cpu.gpr[0]._32=pio_read(port,4);

	else assert(0);

	return 1;
}

