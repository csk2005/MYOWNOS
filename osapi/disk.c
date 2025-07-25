/* disk.c */
#include "disk.h"

internal int8 attached;

internal void dshow(disk *dd){
    if(dd)printf("drive 0x%.02hhx\nfd=%d\nblocks=%d\n\n", (char)dd->drive,$i dd->fd,$i dd->blocks);
    return;
}

internal packed enum {
    TypeNotValid = 0x00,
    TypeFile = 0x01,
    TypeDir = 0x03
};

typedef int8 bootsector[500];

internal struct s_superblock{
    bootsector boot;
    int16 _;
    int16 blocks;
    int16 inodeblocks;
    int16 inodes;
    int16 magic1;
    int16 magic2;
} packed;
typedef struct s_superblock superblock;

internal struct s_filesystem {
    int8 drive;
    disk* dd;
    bool *bitmap;
    superblock metadata;
} packed;
typedef struct s_filesystem filesystem;

internal filesystem *fsmount(int8);

public disk *DiskDescriptor[Maxdrive];
public void dinit() {
    int8 n;
    
    attached = 0;
    for(n=1; n<=Maxdrive; n++){  
       *(DiskDescriptor+ (n-1)) = dattach(n);
    }

    if(*DiskDescriptor) fsmount(1);

    for(n=1; n<=Maxdrive; n++){
       ddetach(DiskDescriptor[n-1]);
    }
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
