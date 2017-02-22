#include "instruction_hash.c"
int main(){
    HashTable * iHT;
    iHT = init_hash_list();
    int i = 0,tmp = 0;
    get_ins();
    for(tmp = 0 ; tmp < 31 ; tmp ++){
        i = match_ins_no(iHT,get_ins());
        printf("%d\n",i);}
    getchar();
}