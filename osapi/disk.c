/* disk.c */
#include "disk.h"

internal int8 attached;

void dinit() {
    attached = 0;
}

void ddetach(disk *dd) {
    int8 x;
    if(!dd) return;
    
    x = ~(dd->drive) & attached;
    attached = x;
    destroy(dd);

    return;
}