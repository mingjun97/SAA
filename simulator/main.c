#include "head.h"
#include "instructions.c"
     //模拟附加数据段
unsigned char *MEM;     //用动态存储区模拟内存，大小由命令行参数确定
unsigned long *PC;      //指令计数器，用来存放下条指令的内存地址
short GR[8];            //通用寄存器的模拟
PROG_STATE_WORD PSW;    //字段结构模拟程序状态字
unsigned long IR;
unsigned long *CS;      //模拟代码段
unsigned char *DS;      //模拟数据段
short *SS;              //模拟堆栈段

/*----------主程序部分----------*/

int main(int argc,char **argv)
{
    unsigned long instruction;
	int length;
	int (*instructions[])(void)={HLT,JMP,CJMP,OJMP,CALL,RET,PUSH,POP,LOADB,LOADW,
	                      STOREB,STOREW,LOADI,NOP,IN,OUT,ADD,ADDI,SUB,
                          SUBI,MUL,DIV,AND,OR,NOR,NOTB,SAL,SAR,EQU,LT,LTE,NOTC};//定义函数指针数组指向指令函数
    FILE* exec_file;
	int ret=1;
	GR[0]=0;
    if (argc == 1) {
        Out_put_Help();
        exit(-1);
    }
/*----------向系统申请动态存储，模拟内存----------*/
	if ((MEM=(unsigned char*)malloc((unsigned int)(1<<24)))==NULL)
    {
		printf("ERROR: fail to allocate memory!\n");
		exit(-1);
	}
	PC=(unsigned long*)MEM;            //使指令计数器指向模拟内存的顶端
	CS=PC;                             //将程序计数器的首地址赋给代码段首地址，保存代码段首地址
	if((exec_file=fopen(argv[1],"r"))==NULL)
    {
		printf("ERROR:cannot open file %s for reading!\n",argv[1]);
		exit(-1);
	}
/*----------从文件中取出目标代码，加载到模拟内存-----------*/
	while(!feof(exec_file))
    {
        fscanf(exec_file,"%li",&instruction);             //从dat文件中读取一行机器码
        memcpy(PC,&instruction,sizeof(instruction)); //复制到PC的模拟内存中
        PC++;
    }
    SS=(short *)PC; //模拟堆栈段
    length=*(PC-1); //用来模拟数据段的中间变量
    DS=(unsigned char *)(PC-(length+3)/4-1);//表示数据段位置
    ES=(struct data *)(MEM+(1<<24)-sizeof(struct data));//附加数据段位置
    PC=(unsigned long *)MEM;
    while(ret)   //模拟处理器执行指令
    {
        IR=*PC; //取指：将PC指示的指令加载到指令寄存器IR
        PC++;
        ret=(*instructions[OPCODE])();//解码并执行指令
    }
    fclose(exec_file);
    free(MEM);
    return 1;

}