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
    int8 *byte;
    bool bit;

    blocks = (pos/8);
    mod = (pos%8);

    byte = str + blocks;
    bit = (bool)getbit_(*byte, mod);

    return bit;
}

internal int16 stringlen(int8 *str) {
    int16 n;
    int8 *p;

    for(p = str, n=0; *p; p++) {
        n++;
    }
    return n;
}

internal void setbit(int8 *str, int16 pos, bool value){
    int16 blocks;
    int8 mod;
    int8 *byte;

    blocks = (pos/8);
    mod = (pos%8);

    byte = str + blocks;
    setbit_(*byte,mod,value);

    return;
}