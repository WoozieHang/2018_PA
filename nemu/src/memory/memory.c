#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>
#include"memory/cache.h"

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
//	printf("paddr:%x,len:%x\n",paddr,len);
	uint32_t ret = 0;
	paddr&=0x3fffffff;
	memcpy(&ret, hw_mem + paddr, len);
//	printf("ret:%x\n",ret);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
#ifdef HAS_DEVICE_VGA
	int num=is_mmio(paddr);
	if(num!=-1)
	{
		return mmio_read(paddr,len,num);
	}
#else
#endif


#ifdef CACHE_ENABLED
	ret=cache_read(paddr,len);
#else
	ret = hw_mem_read(paddr, len);

#endif	
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef HAS_DEVICE_VGA
	int num=is_mmio(paddr);
	if(num!=-1){
		mmio_write(paddr,len,data,num);
		return;
	}
#else
#endif

#ifdef CACHE_ENABLED
	cache_write(paddr,len,data);
#else	
	hw_mem_write(paddr, len, data);

#endif
}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len==1||len==2||len==4);
	if(cpu.cr0.pg==1){
		if((laddr&0xfff)+len>0x1000){
			//data cross the page boundary
			uint32_t low_len=0xfff-(laddr&0xfff)+1;
			uint32_t high_len=len-low_len;
			paddr_t low_paddr=page_translate(laddr);
			paddr_t high_paddr=page_translate(laddr+low_len);

			uint32_t rtn_high=paddr_read(high_paddr,4);
			uint32_t rtn_low=paddr_read(low_paddr,4);
			rtn_high&=~(0xffffffff<<(8*high_len));
			rtn_low&=~(0xffffffff<<(8*low_len));
			uint32_t rtn=(rtn_high<<(8*low_len))|rtn_low;
			return rtn;
		}
		else{
		//	printf("not overline   ");
			paddr_t paddr=page_translate(laddr);
			return paddr_read(paddr,len);
		}	
	
	}

	else return paddr_read(laddr,len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	assert(len==1||len==2||len==4);
	if(cpu.cr0.pg==1){
		if((laddr&0xfff)+len>0x1000){
			//data cross the page boundary
			//printf("overline\n");
		//	assert(0);
			/*
			 uint32_t low_len=0xfff-(laddr&0xfff)+1;
			uint32_t high_len=len-low_len;

			paddr_t low_paddr=page_translate(laddr);
			paddr_t high_paddr=page_translate(laddr+low_len);

			uint32_t high_data=(data>>(8*low_len))&(~(0xffffffff<<(8*high_len)));
			uint32_t low_data=data&(~(0xffffffff<<(8*low_len)));

			paddr_write(high_paddr,high_len,high_data);
			paddr_write(low_paddr,low_len,low_data);
			*/
			paddr_t paddr=page_translate(laddr);
			paddr_write(paddr,len,data);
		}
		else{
			paddr_t paddr=page_translate(laddr);
			 paddr_write(paddr,len,data);
		}	
	
	}

	else paddr_write(laddr, len, data);
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
//	printf("vaddr:%x,len:%x\n",vaddr,len);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else 
	uint32_t laddr=vaddr;
	if(cpu.cr0.pe==1){
		laddr=segment_translate(vaddr,sreg);
	}
	return laddr_read(laddr,len);
#endif

}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	//printf("vaddr:%x,len:%x,data:%x\n",vaddr,len,data);
#ifndef IA32_SEG
	laddr_write(vaddr, len, data);
#else
	uint32_t laddr=vaddr;
	if(cpu.cr0.pe==1){
		laddr=segment_translate(vaddr,sreg);
	}
	laddr_write(laddr,len,data);
#endif
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache();
#endif

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}
