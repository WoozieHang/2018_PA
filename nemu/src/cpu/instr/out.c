#include "cpu/instr.h"

make_instr_func(out_b) {
//int x;
//scanf("%d",&x);
uint16_t port=cpu.gpr[2]._16;
uint32_t data=((uint32_t)(cpu.gpr[0]._8[0]))&0xff;
//printf("port:%x,edx:%x,data:%x,eax:%x\n",port,cpu.edx,data,cpu.eax);
pio_write(port,1,data);
return 1;
}
