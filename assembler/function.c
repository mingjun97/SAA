#include "head.h"
void Out_put_Help(char* name){
    printf("Assembler v1.0\n");
    printf("\tHelp Menu\n");
    printf("usage: %s [options] [sourcefile] [assemblerfile]\n",name);
    printf("Options:\n");
    printf(" -h \t Show This Help Menu\n");
    printf(" -d \t Insert Debug Informations into the assemblerfile.\n");
    printf("\nCoding By Yinmingjun");
    return;
}
void printlog(char * message){//Out put debug logs to somewhere
    printf("%s",message);
}
/**/
char * read_line(FILE * fp){
    #ifdef DEBUG
        return "     test";
    #endif
    if (fp == NULL) return ERROR; //check wether the file open
    //read line from file
}