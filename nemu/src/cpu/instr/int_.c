#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(int_){
	uint8_t intr_no=vaddr_read(cpu.eip+1,1,1);
	raise_sw_intr(intr_no);
	return 0;
}
