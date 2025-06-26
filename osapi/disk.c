/* disk.c */
#include "disk.h"

internal int8 attached;

internal void dshow(disk *dd){
    if(dd)printf("drive 0x%.02hhx\nfd=%d\nblocks=%d\n\n", (char)dd->drive,$i dd->fd,$i dd->blocks);
    return;
}

public void dinit() {
    disk* dd[2];
    block bl = {0};
    bool x;
    

    attached = 0;
    *dd = dattach(1);
    *(dd+1) = dattach(2);


    dshow(*dd);
    dshow(*(dd+1));

    x = dread(*dd, &bl, 2);
    printf("x=%s\n", (x)? "true" : "false");
    printf("0x%.02hhx\n", (char)bl[500]);

    ddetach(*dd);
    ddetach(*(dd+1));

    return;
}

internal void ddetach(disk *dd) {
    int8 x;
    if(!dd) return;

    close(dd->fd);
    
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

    tmp = open($c file, O_RDWR);
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

    dd->blocks = $2 ((sbuf.st_size+Blocksize-1) / Blocksize);
    dd->drive = drive;
    attached |= drive;

    return dd;
}

internal int16 openfiles(disk *dd) {
    return 0;
}
internal void closeallfiles(disk *dd) {
    return;
}
