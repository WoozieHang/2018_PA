#include"cpu/instr.h"
static void instr_execute_2op(){
operand_read(&opr_dest);
operand_read(&opr_src);
if(opr_src.data_size<opr_dest.data_size){
//sign_ext

if(opr_src.val>>(opr_src.data_size-1)&1){

	opr_src.val|=0xFFFFFFFF<<opr_src.data_size;

}

else{

opr_src.val&=0x7FFFFFFF>>(31-opr_src.data_size);

}

}

opr_dest.val=alu_xor(opr_src.val,opr_dest.val,data_size);
operand_write(&opr_dest);
}

make_instr_impl_2op(xor,i,rm,bv)
make_instr_impl_2op(xor,i,rm,v)
make_instr_impl_2op(xor,i,rm,b)
make_instr_impl_2op(xor,r,rm,b)
make_instr_impl_2op(xor,r,rm,v)
make_instr_impl_2op(xor,rm,r,b)
make_instr_impl_2op(xor,rm,r,v)
make_instr_impl_2op(xor,i,a,b)
make_instr_impl_2op(xor,i,a,v)

