#include "head.h"
int main(int argc,char* argv[]){
    int i,line_no=0;
    char status_flag = 0; //flag for wether input the source file
    char * sourcefile,*es;
    char * destionfile;
    FILE * source,*binary;
    if (argc == 1) {
        Out_put_Help();
        exit(-1);
    }
    for(i = 1;i<argc;i++){
        if(*(argv[i])=='-')
            switch(*(argv[i]+1)){
                case '-':
                    break;
                case 'h':
                case 'H'://Print Help Manual.
                    Out_put_Help(argv[0]);
                    break;
                default:
                    printlog("WRONG PARAM! ");
                    printf("%s",argv[i]);
            }
        else if (status_flag == 0) {sourcefile = argv[i];status_flag = 1;}
        else {destionfile = argv[i];status_flag = 2;}
    }
    HashTable* InstructionHashTable;
    InstructionHashTable = init_hash_list();
    source = fopen(sourcefile,"r");
    if(source == NULL) {
        printlog("Can't Open SOURCE_FILE!Please check it wether valid.\n");
        exit(-1);
    }
    binary = fopen(destionfile,"wb");
    if(binary == NULL) {
        printlog("Can't Open DESTINATION_FILE!Please check it wether valid.\n");
        exit(-1);
    }
    cmd* info;
    char* command;
    int counter=0;
    command = malloc(MAX_LEN+1);
    es = malloc(1); *es = 0;
    while(!feof(source)){
        fgets(es,MAX_LEN-1,source);
        if(recognize(command,info,InstructionHashTable,line_no)!=ERROR) {
            line_no++;
            if(info->flag == 0) {
                fwrite(info->instruction,4,1,binary);
            }else{
                deal_symbol(command,info->instruction[0],&counter,es);
            }
        }else{
            exit(-1);
        }
    }
    fwrite(es,((counter - 1)/4 + 1)*4,1,binary);
    fclose(binary);
    fclose(source);
    exit(1);
}
