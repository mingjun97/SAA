#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ERROR -1
#define SUCCESS 1
#define INFO 0
#define MAX_INSTR 34
#define MAX_LEN 500
typedef int status;
typedef struct {
    char flag;
    char instruction[4];
}cmd;
typedef struct HashList{
    char * instruction;
    int instruction_no;
    struct HashList* next;
}HashTable;