#include"cpu/instr.h"
static void instr_execute_2op(){
//printf("dest_data_size:%x,src_data_size:%x\n",opr_dest.data_size,opr_src.data_size);
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
//printf("src:%x,dest:%x,data_size:%x\n",opr_src.val,opr_dest.val,data_size);
opr_dest.val=alu_and(opr_src.val,opr_dest.val,data_size);
//printf("ans:%x\n",opr_dest.val);
operand_write(&opr_dest);
}


make_instr_impl_2op(and,i,rm,v)
make_instr_impl_2op(and,i,rm,b)
make_instr_impl_2op(and,i,rm,bv)
make_instr_impl_2op(and,r,rm,b)
make_instr_impl_2op(and,r,rm,v)
make_instr_impl_2op(and,rm,r,b)
make_instr_impl_2op(and,rm,r,v)
make_instr_impl_2op(and,i,a,b)
make_instr_impl_2op(and,i,a,v)

