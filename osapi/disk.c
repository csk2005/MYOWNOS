/* disk.c */
#include "disk.h"

internal int8 attached;

void dinit() {
    attached = 0;
}

void ddetach(disk *dd) {
    if(!dd) return;

    attached &= ~(dd->drive) & attached;
    free(dd);
    
    return;
}