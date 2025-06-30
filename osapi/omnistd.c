/* omnistd.c */
#include "omnistd.h"

internal void zero(int8 *str, int16 size){
    int8 *p;
    int16 n;
    for(n=size,p=str;n>0;n--,p++) *p=(int8)0;

    return;
}

internal void memorycopy(int8 *dst, int8 *src, int16 len, bool IsString){
    int16 n;
    int8 *dp, *sp;

    for(dp=dst, sp=src, n=len; n>0; n--, dp++, sp++){
        if((IsString)&&!(*sp))break;
        *dp = *sp;
    }
    return;
}

internal bool getbit(int8 *str, int16 pos){
    int16 blocks;
    int8 mod;
    void *mem;
    int8 *byte;
    bool bit;

    blocks = (pos/8);
    mod = (pos%8);

    mem = (void *)str + blocks;
    byte = $1 mem;
    bit = (bool)getbit_(*byte, mod);

    return bit;
}

internal void setbit(int8 *str, int16 pos, bool value){
    int16 blocks;
    int8 mod;
    void *mem;
    int8 *byte;

    blocks = (pos/8);
    mod = (pos%8);

    mem = (void *)str + blocks;
    byte = $1 mem;
    setbit_(*byte,mod,value);

    return;
}