/* disk.c */
#include "disk.h"

internal int8 attached;

void dinit() {
    attached = 0;
}

internal void ddetach(disk *dd) {
    int8 x;
    if(!dd) return;
    
    x = ~(dd->drive) & attached;
    attached = x;
    destroy(dd);

    return;
}

internal disk *dattach(int8 drive){
    disk * dd;
    int16 size;
    int8* file;
    signed int tmp;
    struct stat sbuf;

    if((drive!=1) && (drive!=2))return (disk *)0;

    if(attached & drive) return (disk *)0;

    size = sizeof(struct s_disk);
    dd = (disk *)malloc($i size);
    if(!dd) return (disk *)0;

    zero($1 dd, size);
    file = strnum(Base, drive);

    tmp = open($c file, O_RDONLY);
    if(tmp < 3){
        free(dd);
        return (disk *)0;
    }
    dd->fd = $4 tmp;

    tmp = fstat($i dd->fd, &sbuf);
    if(tmp || !sbuf.st_size){
        close(dd->fd);
        free(dd);

        return (disk *)0;
    }

    dd->blocks = $2 (sbuf.st_size+Blocksize-1) / Blocksize;
    dd->drive = drive;
    attached |= drive;

    return dd;
}