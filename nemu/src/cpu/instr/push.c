#include"cpu/instr.h"
static void instr_execute_1op(){

	OPERAND s;
	//cpu.esp-=data_size/8;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp-data_size/8;
	s.data_size=data_size;
	operand_read(&opr_src);
	if(opr_src.data_size==8){
	//sign_ext

		if(opr_src.val>>(opr_src.data_size-1)&1)
			opr_src.val|=0xFFFFFFFF<<opr_src.data_size;
		else
			opr_src.val&=0x7FFFFFFF>>(31-opr_src.data_size);

	}
	s.val=opr_src.val;
//	printf("val:%x\n",s.val);
	cpu.esp-=data_size/8;
	operand_write(&s);
}


make_instr_impl_1op(push,r,v)
make_instr_impl_1op(push,rm,v)
make_instr_impl_1op(push,i,v)
make_instr_impl_1op(push,i,b)

make_instr_func(pusha) {
	uint32_t temp=cpu.esp;
	//push eax
	OPERAND s;
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.eax;
	operand_write(&s);
	//push ecx
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.ecx;
	operand_write(&s);
	//push edx
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.edx;
	operand_write(&s);
	//push ebx
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.ebx;
	operand_write(&s);
	//push temp
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=temp;
	operand_write(&s);
	//push ebp
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.ebp;
	operand_write(&s);
	//push esi
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.esi;
	operand_write(&s);

	//push edi
	cpu.esp-=4;
	s.sreg=SREG_DS;
	s.type=OPR_MEM;
	s.addr=cpu.esp;
	s.data_size=32;
	s.val=cpu.edi;
	operand_write(&s);
	
	return 1;
}
