#include"memory/cache.h"
#include"memory/memory.h"
#include"time.h"
#include"stdlib.h"
#include"nemu.h"

CacheLine myCache[1024];



void init_cache(){
for(int i=0;i<1024;i++)
	myCache[i].valid_bit=0;
}

uint32_t cache_read(paddr_t paddr,size_t len){
	//printf("cache read*\n");
	//printf("right ans reference:%x,%x,%x,%x\n",hw_mem[paddr],hw_mem[paddr+1],hw_mem[paddr+2],hw_mem[paddr+3]);

//	printf("paddr:%x,len:%x\n",paddr,len);
	paddr_t padr=paddr;
	uint32_t rtn=0;
	uint32_t tag;
	uint32_t grp;
	uint32_t block_addr;
	block_addr=padr%64;
	padr>>=6;
	grp=padr%128;
	padr>>=7;
	tag=padr%16384;
//	printf("tag:%x,grp:%x,block_addr:%x\n",tag,grp,block_addr);
	CacheLine *index=&myCache[grp*8];
	bool hit=0;
	for(int i=0;i<8;i++){
		if(index->valid_bit==1&&index->tag==tag){
			hit=1;//hit!
			break;
			//printf("hit and index:%x,%x,%x\n",index->valid_bit,index->tag,index->data[block_addr]);
		}
		index++;
	}	
	//hit
	if(hit){
		//if not overline
		if(block_addr+len<=64){
		//	printf("hit and not overline\n");
			memcpy(&rtn,&index->data[block_addr],len);
		}
		//if overline
		else{
		//	printf("hit and overline\n");
			uint32_t byte_thisline=64-block_addr;
			//printf("byte_thisline:%x\n",byte_thisline);
			memcpy(&rtn,&index->data[block_addr],byte_thisline);
			//printf("hit and then going into the next line\n");
			uint32_t nextline_rtn=cache_read(paddr+byte_thisline,len-byte_thisline);
			//printf("now we have finish the next line\n");
			//printf("firstline:%x,nextline:%x\n",rtn,nextline_rtn);
			nextline_rtn<<=byte_thisline*8;
			rtn|=nextline_rtn;
			//printf("nextline:%x\n",nextline_rtn);
		}
	
	}
	
	//miss
	else{
		index=&myCache[grp*8];
		//find a positon to save the data of memory
		bool have_position=0;
		for(int i=0;i<8;i++){
			if(index->valid_bit==0){
				have_position=1;
				break;
		//		printf("miss and have positon\n");
			}
			index++;
		}

		//if position is availble,the index is now pointing to that space!
		//so do nothing
		//else if no position then we need to exchange by random
		if(!have_position){
			srand((unsigned)time(0));
			//exchange by random
			uint32_t aRand=grp*8+rand()%8;
			index=&myCache[aRand];		
		//	printf("miss and choose random postion:%x\n",aRand);
		}

		index->valid_bit=1;
		index->tag=tag;
		memcpy(&rtn,&hw_mem[paddr],len);
		//paddr-block_addr is the head address of the block
		memcpy(index->data,&hw_mem[paddr-block_addr],64);
		uint32_t byte_thisline=64-block_addr;
		//if length of all bytes number is larger than bytes number of this line,then we need to save the data over line
		if(len>byte_thisline){
		//	printf("miss and now going into the next line\n");
			cache_read(paddr+byte_thisline,len-byte_thisline);
		//	printf("now we have finish the next line\n");

		}//else do nothing
		//

	}
	//printf("ans:%x\n\n",rtn);
	return rtn;
}




void cache_write(paddr_t paddr,size_t len,uint32_t data){
	//printf("cache write*\n");
	//printf("paddr:%x,len:%x,data:%x\n",paddr,len,data);
	//printf("before write reference:%x,%x,%x,%x\n",hw_mem[paddr],hw_mem[paddr+1],hw_mem[paddr+2],hw_mem[paddr+3]);

	paddr_t padr=paddr;
	uint32_t tag;
	uint32_t grp;
	uint32_t block_addr;
	block_addr=padr%64;
	padr>>=6;
	grp=padr%128;
	padr>>=7;
	tag=padr%16384;

	//printf("tag:%x,grp:%x,block_addr:%x\n",tag,grp,block_addr);
	
	CacheLine *index=&myCache[grp*8];
	bool hit=0;
	for(int i=0;i<8;i++){
		if(index->valid_bit==1&&index->tag==tag){
			hit=1;//hit!
			break;
			//printf("hit and index:%x,%x,%x\n",index->valid_bit,index->tag,index->data[block_addr]);

		}
		index++;
	}

	memcpy(&hw_mem[paddr],&data,len);

	//if hit, updata the cache as well:write through
	if(hit){

		memcpy(index->data,&hw_mem[paddr-block_addr],64);
		uint32_t byte_thisline=64-block_addr;
		//if length of all bytes number is larger than bytes number of this line,then we need to save the data over line
		if(len>byte_thisline){
			//printf("hit and now going to the next line\n");
			cache_write(paddr+byte_thisline,len-byte_thisline,data>>(byte_thisline*8));
			//printf("now we have finish the next line\n");
		}
		
	}

	//else do nothing: not write allocate
	//else printf("not hit and do nothing\n");
	//printf("after write reference:%x,%x,%x,%x\n\n",hw_mem[paddr],hw_mem[paddr+1],hw_mem[paddr+2],hw_mem[paddr+3]);


}
