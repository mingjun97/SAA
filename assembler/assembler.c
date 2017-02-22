#include "head.h"
static HashTable* symbol_alias_table = NULL; // 用于储存伪指令所定义的变量
int assembler_error(char * command,int pos,int line_no){
    printf("[!]Error Occured \nThis will help you:\n%d:\n\t%s\n\t",line_no,command);
    for(pos-=1;pos>1;pos--) putchar(' ');
    putchar('^');
    return 1;
}
status recognize(char * command ,cmd* info,HashTable* hashtable,int line_no){
    static int ass_line = 0; //use for store the line assembled.
    static HashTable* address_alias_table = NULL; // table for ass_line

    if(address_alias_table == NULL) { //init hash table
        address_alias_table = malloc(sizeof(HashTable)*26);
        symbol_alias_table = malloc(sizeof(HashTable)*26);
        int i;
        for ( i = 0 ;i < 26 ; i ++) address_alias_table[i].next = symbol_alias_table[i].next = NULL;
    }
    int pos=0;
    int i = 0 ;
    char instruction[50];
    int param1,param2,param3;
    char instruction_no;
    char status_flag = 0;
    /* status_flag
        0 null
        1 instruction recognized
        2 first param recognized
        3 second param recognized 
        .....
    */
    while(*(command + pos) != '\n'){ //mark the commit command as the end of the line
        if (*(command + pos) == ':') {
            if (add_alias(address_alias_table,command,ass_line)==ERROR) return ERROR;
            command = command + pos+1;
        }
        if (*(command + pos) == '#') {*(command + pos) = '\n';break;}
        pos ++;
    }
    pos = 0;
    while(*(command + pos) != '\n'){
        while(*(command + pos) == ' ' || *(command + pos)=='\t') pos++; //跳过空白符
        i = 0;
        while(*(command + pos) != ' ' && *(command + pos)!='\t' && *(command + pos) != '\n') { //读取指令，直到行末或下一个空白符
            instruction[i] = *(command+pos);
            i++;
            pos++;
        }
        instruction[i] = '\0'; // 将指令末尾标记字符串结束
        if(status_flag == 0 && i!=0) { //成功获取到了当前指令（排除空行以及纯注释行）
            instruction_no = match_ins_no(hashtable,instruction); //在哈希表中查找指令代码
            if ( instruction_no < 0 ) { //指令不存在，返回错误
                assembler_error(command,pos,line_no);
                printlog("\nInvalid instruction!");
                return ERROR;
            } 
            if ( instruction_no == 32) { //如果是伪指令则进行返回main函数进行处理
                info->flag = 1;
                info->instruction[0] = (char)instruction_no;
                return SUCCESS;
            }
            status_flag ++;  //标记当前状态
        }else if(status_flag == 1 && i!=0){ 
        //对一个参数进行处理
            status_flag++;
            switch (instruction_no){
                //type one for only one instruction
                case 0: case 5: case 13 : case 31:
                    assembler_error(command,pos,line_no);
                    printlog("\nToo many params!");
                    return ERROR;
                //type two for jump to an address
                case 1:case 2 : case 3 : case 4:
                    param1 = find_alias(address_alias_table,instruction);
                    if(param1 == ERROR) {
                        assembler_error(command,pos,line_no);
                        printlog("Can't find address!Please check the source code.\n");
                        return ERROR;
                    }
                    param1 *= 4;
                    break;
                //type 3/4/5/6/7/8 for reg0
                default:
                    param1 = match_reg(instruction);
                    if(param1 == ERROR) {
                        assembler_error(command,pos,line_no);
                        printlog("Wrong REG!Please check the source code.\n");
                        return ERROR;
                    }
            }
        }else if(status_flag == 2 && i!=0){
        //对第二个参数进行处理
            status_flag ++;
            switch (instruction_no){
                //类型 1/2 报错，参数过多
                case 0: case 5: case 13: case 31: case 1: case 2: case 3: case 4:
                    assembler_error(command,pos,line_no);
                    printlog("\nToo many params!");
                    return ERROR;
                //类型 4 获得伪指令位置代码
                case 8: case 9: case 10: case 11:
                    param2 = find_alias(symbol_alias_table,instruction);
                    param3 = strchr(command,'[');
                    if(param2 == ERROR) {
                        assembler_error(command,pos,line_no);
                        printlog("Can't find this address!");
                        return ERROR;
                    }
                    sscanf(command+param3,"%d",&param3);
                    if(param3!=NULL)
                        switch(instruction_no){
                            case 8: case 10:
                                param2 += param3;
                                break;
                            case 9: case 11:
                                param2 += param3 << 1;
                                break;
                        }
                    break;
                //类型 5/6 接受10进制数字;
                case 12: case 17: case 19: case 14: case 15:
                    i = 0;
                    while(*(instruction+i)!= '\0')
                        if(*(instruction+i) < '0' || *(instruction + i) > '9') {
                            assembler_error(command,pos,line_no);
                            printlog("Wrong param! It must be DECIMAL NUMBER!");
                            return ERROR;
                        }
                    sscanf(instruction,"%d",param2);
                    break;
                //类型 7/8 接受 reg1
                case 16: case 18: case 20: case 21: case 22: case 23: case 24: case 26: case 27:
                case 25: case 28: case 29: case 30:
                    param2 = match_reg(instruction);
                    if(param2 == ERROR) {
                        assembler_error(command,pos,line_no);
                        printlog("Wrong REG!Please check the source code.\n");
                        return ERROR;
                    }
            }
        }else if(status_flag == 3 && i!=0){
        //对参数3进行处理
            status_flag ++;
            switch(instruction_no){
                //类型 7 接受 reg2
                case 16: case 18: case 20: case 21: case 22: case 23: case 24: case 26: case 27:
                    param3 = match_reg(instruction);
                    if(param3 == ERROR) {
                        assembler_error(command,pos,line_no);
                        printlog("Wrong REG!Please check the source code.\n");
                        return ERROR;
                    }
                    break;
                default:
                    assembler_error(command,pos,line_no);
                    printlog("\nToo many params!");
                    return ERROR;
            }
        }
    }
    info->flag = 0;
    ass_line++; //add and sign line
    switch (instruction_no){
        //type one
        case 0: case 5: case 13: case 31: //HLT RET NOP NOTC
            info->instruction[0] = ((char)instruction_no << 3) & 0xf8;
            return SUCCESS;
            break;
        //type two
        case 1: case 2: case 3: case 4:
            if(status_flag != 2) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = ((char)instruction_no << 3) & 0xf8;
            info->instruction[1] = ((char)(param1 & 0xff0000)>>16);
            info->instruction[2] = ((char)(param1 & 0x00ff00)>>8);
            info->instruction[3] = ((char)(param1 & 0x0000ff));
            return SUCCESS;
            break;
        //type three
        case 6: case 7:
            if(status_flag != 2) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = info->instruction[2] = info->instruction[3] = 0;
            break;
        //type four
        case 8: case 9: case 10: case 11:
            if(status_flag != 3) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = ((char)(param2 & 0xff0000)>>16);
            info->instruction[2] = ((char)(param2 & 0x00ff00)>>8);
            info->instruction[3] = ((char)(param2 & 0x0000ff));
            break;
        //type five
        case 12: case 17: case 19:
            if(status_flag != 3) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = 0;
            info->instruction[2] = ((char)(param2 & 0xff00)>>8);
            info->instruction[3] = ((char)(param2 & 0x00ff));
            break;
        //type six
        case 14: case 15:
            if(status_flag != 3) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = 0;
            info->instruction[2] = 0;
            info->instruction[3] = ((char)(param2 & 0x00ff));
            break;
        //type seven
        case 16: case 18: case 20: case 21: case 22: case 23: case 24: case 26: case 27:
            if(status_flag != 4) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = ((param2 << 4 ) & 0xf0)+ (param3 & 0x0f);
            info->instruction[2] = info->instruction[3] = 0;
            break;
        //type eight
        case 25: case 28: case 29: case 30:
        if(status_flag != 3) {
                assembler_error(command,0,line_no);
                printlog("No enough param!");
                return ERROR;
            }
            info->instruction[0] = (((char)instruction_no << 3) & 0xf8 ) + param1;
            info->instruction[1] = ((param2 << 4 ) & 0xf0);
            info->instruction[2] = info->instruction[3] = 0;
            break;
    }
    if(pos == 0) return SUCCESS;
    return SUCCESS;
}
status add_alias(HashTable* alias_table,char* alias,int line_no){
    while(*alias == ' ' || *alias == '\t') alias++;
    char first_char = 0;
    HashTable * tHT;
    first_char = *alias;
    if(first_char >= 'a' && first_char <= 'z') first_char-='a';
    else if(first_char >= 'A' && first_char<='Z') first_char -= 'A';
    else return ERROR;
    tHT = alias_table + first_char;
    while(tHT->next!=NULL) tHT=tHT->next;
    tHT->next = malloc(sizeof(HashTable));
    int alias_len = 0;
    while(*(alias + alias_len) != ' ' && *(alias + alias_len) != '\t' && *(alias + alias_len) != ':') alias_len++;
    tHT->instruction = malloc(alias_len + 1);
    tHT->instruction_no = line_no;
    memcpy(tHT->instruction,alias,alias_len);
    alias[alias_len] = '\0';
    return SUCCESS;
}
int find_alias(HashTable* alias_table,char * alias){
    int t;
    char first_char=0;
    if(*alias >= 'A' && *alias <= 'Z') first_char -= 'A';
    else if (*alias >= 'a' && *alias <= 'z') first_char -= 'a';
    else return ERROR;
    if(alias_table == NULL) return ERROR;
    HashTable * tHT;
    tHT = alias_table + first_char;
    while(tHT != NULL) {
        if (strcmp(tHT->instruction,alias)==0) return tHT->instruction_no;
        tHT = tHT->next;
    }
    return ERROR;
}
int match_reg(char *reg_name){
    if(*reg_name == 'Z') return 0;
    if(*reg_name < 'A' || *reg_name > 'G') return ERROR;
    if(*(reg_name + 1) != '\0') {
        printlog("No Reg Named ");
        printf("%s",reg_name);
        return ERROR;
    }
    return *reg_name - 'A' + 1;
}
status deal_symbol(char * command,char flag,int * counter,char* memory){
    char alias[50];
    int tmp=0;
    if(flag == 32) flag = 1; //BYTE类型
    else flag = 2; //WORD类型
    int counter_old,element=0;
    counter_old = * counter;
    command+=4;
    while(*(command)==' ' || *(command) == '\t') command++;//略过开头的空格
    while(*(command+tmp) != ' ' && *(command+tmp)!= '\t' && *(command+tmp)!= '=' && *(command+tmp)!='\n') tmp++; //计算symbol长度
    memcpy(alias,command,tmp); //拷贝至alias变量中，以便后期取用
    alias[tmp] = '\0';
    add_alias(symbol_alias_table,alias,counter); //添加记录
    if((tmp = strchr(command,'['))!= NULL) { //判断是否是数组形式
        command = tmp+1;
        sscanf(command,"%d",&tmp);
        element = tmp;
        *counter += flag * tmp;
    }else{ //不是数组形式
        *counter += flag;
    }
    memory=realloc(memory,*counter); //重新分配缓存大小
    if((tmp = strchr(command,'='))!=NULL){ //判断是否有等号
        //有等号
        command = tmp + 1;
        if((tmp = strchr(command,'{'))!=NULL){//如果有大括号
            int i=0;
            for(command = tmp;i< element;i++){
                command++; //略去逗号
                sscanf(command,"%d",&tmp); //读取数值
                if(flag == 1) *(memory+counter_old+i) = (char)tmp;//根据数据类型进行操作
                else{
                    *(memory+counter_old+(i<<1)) = (char) tmp & 0xff00 >> 16;
                    *(memory+counter_old+(i<<1) + 1) = (char) tmp & 0xff;
                }
                if((command = strchr(command,','))==NULL) {assembler_error("FATAL ERROR! No enough param for BYTE/WORD",0,0); return ERROR;}
            }
        }else{ //无大括号
            sscanf(command,"%d",&tmp);
            if(flag == 1) *(memory+counter_old) = (char)tmp;
            else{
                *(memory+counter_old) = (char) tmp & 0xff00 >> 16;
                *(memory+counter_old+1) = (char) tmp & 0xff;
            }
        }
    }else{
        //无等号
        memset(memory + counter_old,'\0', counter - counter_old);
    }
}