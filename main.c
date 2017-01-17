#include "head.h"
int main(int argc , char* argv[]){//Main
    int tmp;
    char debug_flag,ofp_flag;
    char* ofp,dfp;
    //flag 
    for(tmp=1;tmp <= argc;tmp++){
        if(*argv[tmp]=='-'){
            switch(*(argv[tmp]+1)){
                case 'h':
                    printf("File");
                    break;
                case 'd':
                    debug_flag = 1;
                    break;
            }
        }
    }