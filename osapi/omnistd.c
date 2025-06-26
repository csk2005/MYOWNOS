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