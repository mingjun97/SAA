//test pass 2017-02-09 15:50:12
#include "head.h"
char * get_ins(){ //return the instruction location in memory
    static int counter=0;
    static char*insTable[]={"HLT","JMP","CJMP","OJMP","CALL","RET","PUSH","POP","LOADB","LOADW","STOREB","STOREW","LOADI","NOP",
        "IN","OUT","ADD","ADDI","SUB","SUBI","MUL","DIV","AND","OR","NOR","NOTB","SAL","SAR","EQU","LT","LTE","NOTC","BYTE","WORD"};
    counter ++;
    if(counter <= MAX_INSTR) return insTable[counter - 1];
    else 
    counter = 0;
    return NULL;
}
// init the hash table for instructions;
HashTable * init_hash_list(){
    HashTable *HT,*pHT;
    int i=0;
    char * ins;
    HT = malloc(sizeof(HashTable)*26);
    for(i=0;i<MAX_INSTR;i++) {HT[i].instruction = NULL;HT[i].next = NULL;}
    i = 0;
    while(i<MAX_INSTR){
        ins = get_ins();
        pHT = HT + *ins - 'A';
        if(pHT->instruction != NULL) {
            while (pHT->next!=NULL) pHT=pHT->next;
            pHT->next = malloc(sizeof(HashTable));pHT = pHT->next;
            pHT->instruction = ins;
            pHT->instruction_no = i;
            pHT->next = NULL;
        }else{
            pHT->instruction = ins;
            pHT->instruction_no = i;
            pHT->next = NULL;
        }
        i++;
    }
    return HT;
}
int match_ins_no(HashTable * HT,char* instruction){
    char first_char;
    HashTable *tHT;
    int pos=0;
    while(*(instruction + pos) != '\0') {
        if(*(instruction + pos) - 'a'>=0 && *(instruction + pos) - 'a'<=26)
            *(instruction + pos) = *(instruction + pos) - 'a' + 'A';//UPCASE the letters
        pos++;
        }
    first_char = *instruction - 'A';
    if(strcmp(HT[first_char].instruction,instruction) == 0) return HT[first_char].instruction_no;
    else {
        tHT = HT + first_char;
        tHT = tHT -> next;
        while (tHT != NULL) if(strcmp(tHT->instruction,instruction)==0) return tHT->instruction_no;
                else tHT = tHT -> next;
        return -1;
    }
}
