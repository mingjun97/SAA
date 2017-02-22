int HLT(void)//停机指令HLT：终止程序运行
{
    return 0;
}

int JMP(void)//无条件转移指令JMP：转移至标号
{
    PC=CS+ADDRESS/4;
    return 1;
}

int CJMP(void)//比较运算转移指令CJMP：c为1则转移到标号
{
    if(PSW.compare_flg)
        PC = CS + ADDRESS/4;
    return 1;
}

int OJMP(void)//溢出转移指令OJMP：o为1则转移到标号
{
    if(PSW.overflow_flg)
       PC = CS + ADDRESS/4;
    return 1;
}

int CALL(void)//调用子程序指令CALL：将寄存器，状态字，PC的值保存到ES，再转移
{
    int i;
    for(i=0;i<8;i++)
        ES->EGR[i]=GR[i];
    ES->EPC=PC;
    ES->EPSW=PSW;
    PC=CS+ADDRESS/4;
    ES--;
    return 1;
}

int RET(void)//子程序返回指令RET：将寄存器，状态字，PC的值恢复，回到被调用处
{
    int i;
    ES++;
    for(i=0;i<8;i++)
       GR[i]=ES->EGR[i];
    PC=ES->EPC;
    PSW=ES->EPSW;
    return 1;
}

int PUSH(void)//入栈指令PUSH
{
    *SS=GR[REG0];
    SS++;
    return 1;
}

int POP(void)//出栈指令POP
{
    SS--;
    GR[REG0]=*SS;

    return 1;
}

int LOADB(void)//取字节数据指令：LOADB
{
    int num=GR[7];
    int shifit=(int)(ADDRESS + num);
    GR[REG0]=(short)(*(DS + shifit));
    return 1;
}

int LOADW(void)//取双字节数据指令：LOADW
{
    int num=GR[7];
    int shifit=(int)(ADDRESS+2*num);
    GR[REG0]=*(short*)(DS+shifit);
    return 1;
}

int STOREB(void)//存字节数据指令：STOREB
{
    int num=GR[7];
    int shifit=(int)(ADDRESS+num);
    *(DS+shifit)=GR[REG0];
    return 1;
}

int STOREW(void)//存双字节数据指令：STOREW
{
    int num=GR[7];
    int shifit=(int)(ADDRESS+2*num);
    *(short *)(DS+shifit)=GR[REG0];
    return 1;
}

int LOADI(void)//取立即数指令：LOADI
{
    GR[REG0]=(short)(IMMEDIATE);
    return 1;
}

int NOP(void)//空操作指令：NOP
{
    return 1;
}

int IN(void)//控制台输入指令：IN，从输入端口取一个字符数据，存入寄存器
{
    read(0,(int *)(GR+REG0),1);
    return 1;
}

int OUT(void)//控制台输出指令：OUT，将数据输出到显示器
{
    write(1,(int *)(GR+REG0),1);
    return 1;
}

int ADD(void)//加运算指令：ADD，要注意溢出标志位的变化
{
    GR[REG0]=GR[REG1]+GR[REG2];
    if(GR[REG2]>0)
    {
        if(GR[REG0]<GR[REG1])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else if(GR[REG2]<0)
    {
        if(GR[REG0]>GR[REG1])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else
        PSW.overflow_flg=0;

    return 1;
}

int ADDI(void)//加立即数指令：ADDI
{

    short i=GR[REG0]+IMMEDIATE;
    if(IMMEDIATE>0)
    {
        if(i<GR[REG0] )
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else if(IMMEDIATE<0)
    {
        if(i>GR[REG0])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else
        PSW.overflow_flg=0;
    GR[REG0]=i;
    return 1;
}

int SUB(void)//减运算指令：SUB
{
    GR[REG0]=GR[REG1]-GR[REG2];
    if(GR[REG2]>0)
    {
        if(GR[REG0]>GR[REG1])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else if(GR[REG2]<0)
    {
        if(GR[REG0]<GR[REG1])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else
        PSW.overflow_flg=0;
    return 1;
}

int SUBI(void)//减立即数指令：SUBI
{
    short i=GR[REG0]-IMMEDIATE;
    if(IMMEDIATE>0)
    {
        if(i>GR[REG0])
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else if(IMMEDIATE<0)
    {
        if(i<GR[REG0])
           PSW.overflow_flg=1;
        else
           PSW.overflow_flg=0;
    }
    else
        PSW.overflow_flg = 0;
    GR[REG0]=i;

    return 1;
}

int MUL(void)//乘运算指令：MUL
{
    GR[REG0]=GR[REG1]*GR[REG2];
    if(abs(GR[REG2])>1)
    {
        if(abs(GR[REG0])<abs(GR[REG1]))
            PSW.overflow_flg=1;
        else
            PSW.overflow_flg=0;
    }
    else
        PSW.overflow_flg=0;

    return 1;
}

int DIV(void)//除运算指令：DIV
{
    GR[REG0]=GR[REG1]/GR[REG2];
    return 1;
}

int AND(void)//按位与运算指令：AND
{
    GR[REG0]=GR[REG1]&GR[REG2];
    return 1;
}

int OR(void)//按位或运算指令：OR
{
    GR[REG0]=GR[REG1]|GR[REG2];
    return 1;
}

int NOR(void)//按位异或运算指令：NOR
{
    GR[REG0]=GR[REG1]^GR[REG2];
    return 1;
}

int NOTB(void)//按位取反运算指令：NOTB
{
    GR[REG0]=~GR[REG1];
    return 1;
}

int SAL(void)//算术左移运算指令：SAL
{
    GR[REG0]=GR[REG1]<<GR[REG2];
    return 1;
}

int SAR(void)//算术右移运算指令：SAR
{
    GR[REG0]=GR[REG1]>>GR[REG2];
    return 1;
}

int EQU(void)//相等关系运算指令：EQU
{
    PSW.compare_flg=(GR[REG0]==GR[REG1]);
    return 1;
}

int LT(void)//小于关系运算指令：LT
{
    PSW.compare_flg=(GR[REG0]<GR[REG1]);
    return 1;
}

int LTE(void)//小于等于关系运算指令：LTE
{
    PSW.compare_flg=(GR[REG0]<=GR[REG1]);
    return 1;
}

int NOTC(void)//比较标志位取反指令：NOTC
{
    PSW.compare_flg=!PSW.compare_flg;
    return 1;
}