#include<stdio.h>
#include<stdlib.h>
#include<linux/io.h>
#include<string.h>
#include<math.h>
/*----------定义宏来模拟指令的解码----------*/
#define REG0 ((IR>>24)&0x0F)
#define REG1 ((IR>>20)&0x0F)
#define REG2 ((IR>>16)&0x0F)
#define IMMEDIATE (IR&0xFFFF)
#define ADDRESS (IR&0xFFFFFF)
#define PORT (IR&0xFF)
#define OPCODE ((IR>>28)&0x1F)//定义指令操作码

typedef struct _PROG_STATE_WORD
{
    unsigned short overflow_flg:1;
    unsigned short compare_flg:1;
    unsigned short reserve:14;
}PROG_STATE_WORD;//定义结构体表示两个标志位

struct data
{
    PROG_STATE_WORD EPSW;
    unsigned short EGR[8];
    unsigned long *EPC;
}*ES;              