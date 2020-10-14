#include "cpu/cpu.h"
void set_CF_add(uint32_t result, uint32_t src, size_t data_size);
void set_CF_adc(uint32_t result,uint32_t src,size_t data_size);
void set_CF_sub(uint32_t dest,uint32_t src,size_t data_size);
void set_CF_sbb(uint32_t dest,uint32_t src,size_t data_size);
void set_CF_mul(uint64_t res,size_t data_size);
void set_CF_shl(uint32_t dest,uint32_t src,size_t data_size);
void set_CF_shr(uint32_t dest,uint32_t src,size_t data_size);
void set_CF_sar(uint32_t dest,uint32_t src,size_t data_size);
void set_ZF(uint32_t result, size_t data_size);
void set_SF(uint32_t result, size_t data_size);
void set_PF(uint32_t result);
void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size);
void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size);
void set_OF_mul(uint64_t res,size_t data_size);


uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
uint32_t res = 0;
res = dest + src;
set_CF_add(res, src, data_size);
set_PF(res);
// set_AF(); 我们不模拟AF
 set_ZF(res, data_size);
 set_SF(res, data_size);
 set_OF_add(res, src, dest, data_size);
return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
uint32_t res = 0;
res = dest + src+cpu.eflags.CF;
set_CF_adc(res, src, data_size);
set_PF(res);
set_ZF(res, data_size);
set_SF(res, data_size);
set_OF_add(res, src, dest, data_size);
return res & (0xFFFFFFFF >> (32 - data_size));
}


uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
uint32_t res=0;
res=dest-src;
set_CF_sub(dest,src,data_size);
set_PF(res);
set_ZF(res,data_size);
set_SF(res,data_size);
set_OF_sub(res,src,dest,data_size);
return res&(0xFFFFFFFF>>(32-data_size));
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
uint32_t res=0;
res=dest-src-cpu.eflags.CF;
set_CF_sbb(dest,src,data_size);
set_PF(res);
set_ZF(res,data_size);
set_SF(res,data_size);
set_OF_sub(res,src,dest,data_size);
return res&(0xFFFFFFFF>>(32-data_size));
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t res=0;
	res=(uint64_t)dest*(uint64_t)src;
	set_CF_mul(res,data_size);
	set_PF(res);
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_OF_mul(res,data_size);
	return res&(0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	int64_t res=(int64_t)src*(int64_t)dest;
	return res;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	if(src==0)
		assert(0);
	else
	{
	uint32_t res=0;
	res=dest/src;
	return res;
	}
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	if(src==0)
		assert(0);
	else
	{
		int32_t res=0;
		res=dest/src;
		return res;
	}
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	return (uint32_t)(dest%src);
}

int32_t alu_imod(int64_t src, int64_t dest) {
	return (int32_t)(dest%src);
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	//printf("src:%x,dest:%x,data_size:%x,res:%x\n",src,dest,data_size,res);
	res=dest&src;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest^src;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res& (0xFFFFFFFF>> (32-data_size));
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest|src;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res& (0xFFFFFFFF>> (32-data_size));

}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest<<src;
	set_CF_shl(dest,src,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	res=res& (0xFFFFFFFF>> (32-data_size));
	return res;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest>>src;
	res&=(0xFFFFFFFF>>(32-data_size+src));
	set_CF_shr(dest,src,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	res=res& (0xFFFFFFFF>> (32-data_size));
	return res;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest>>src;
	uint32_t sgn=1&(dest>>(data_size-1));
	if(sgn==0)
	{
		res&=(0xFFFFFFFF>>(32-data_size+src));
	}
	else
	{
		res|=(0xFFFFFFFF<<(data_size-src));
	}
	set_CF_sar(dest,src,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	res=res& (0xFFFFFFFF>> (32-data_size));
	return res;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t res=0;
	res=dest<<src;
	set_CF_shl(dest,src,data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	res=res& (0xFFFFFFFF>> (32-data_size));
	return res;
}


void set_CF_add(uint32_t result,uint32_t src,size_t  data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result,uint32_t src,size_t data_size){
	result =sign_ext(result & (0xFFFFFFFF>>(32-data_size)),data_size);
	src=sign_ext(src &(0xFFFFFFFF>>(32-data_size)),data_size);
	if(src+cpu.eflags.CF>=src&&result>=src+cpu.eflags.CF)
		cpu.eflags.CF=0;
	else cpu.eflags.CF=1;

}

void set_CF_sub(uint32_t dest,uint32_t src,size_t data_size){

	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(dest<src)
		cpu.eflags.CF=1;
	else cpu.eflags.CF=0;
}

void set_CF_sbb(uint32_t dest,uint32_t src,size_t data_size){
	dest=sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src=sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(dest<src)
		cpu.eflags.CF=1;
	else if(dest==src)
		;
	else cpu.eflags.CF=0;
}


void set_CF_mul(uint64_t res,size_t data_size)
{
	
	res&= (0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
	uint64_t x=res;
	x>>=data_size;
	if(x!=0)
	{cpu.eflags.CF=1;
	}
	else {cpu.eflags.CF=0;
	}
}

void set_CF_shl(uint32_t dest,size_t src,size_t data_size){

	dest=sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src=sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(src==0)
	cpu.eflags.CF=0;
	else
	{uint32_t res=dest<<(src-1);
	uint32_t x=1<<(data_size-1);
	x=x&res;
	if(x==0)
		cpu.eflags.CF=0;
	else cpu.eflags.CF=1;
	}
}

void set_CF_shr(uint32_t dest,size_t src,size_t data_size){
	dest=sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src=sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(src==0)
		cpu.eflags.CF=0;
	else{
	uint32_t res=dest>>(src-1);
	res&=1;
	if(res)
		cpu.eflags.CF=1;
	else cpu.eflags.CF=0;
	}
}

void set_CF_sar(uint32_t dest,size_t src,size_t data_size){
	dest=sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src=sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(src==0)
		cpu.eflags.CF=0;
	else{
	uint32_t res=dest>>(src-1);
	res&=1;
	if(res)
		cpu.eflags.CF=1;
	else cpu.eflags.CF=0;
	}
}

void set_ZF(uint32_t result,size_t data_size){

	result = result & (0xFFFFFFFF >> (32 - data_size));

	cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result,size_t data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result){
int num=0;
for(int i=0;i<8;i++)
{
if(result%2!=0)
	num++;
result/=2;
}
if(num%2==0)
	cpu.eflags.PF=1;
else cpu.eflags.PF=0; 
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
switch(data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
}
if(sign(src) == sign(dest)) {
	if(sign(src) != sign(result))
		cpu.eflags.OF = 1;
	else
		cpu.eflags.OF = 0;
} else {
	cpu.eflags.OF = 0;
}
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8:
			result = sign_ext(result & 0xFF, 8);
			src = sign_ext(src & 0xFF, 8);
			dest = sign_ext(dest & 0xFF, 8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF, 16);
			src = sign_ext(src & 0xFFFF, 16);
			dest = sign_ext(dest & 0xFFFF, 16);
			break;
		default: break;// do nothing
	}
	if(sign(src)!= sign(dest)) {
		if(sign(dest) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	} else {
		cpu.eflags.OF = 0;
	}
}

void set_OF_mul(uint64_t res,size_t data_size)
{
	res&= (0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
	uint64_t x=res;
	x>>=data_size;
	if(x!=0)
	{cpu.eflags.OF=1;
	}
	else {cpu.eflags.OF=0;
	}
}


