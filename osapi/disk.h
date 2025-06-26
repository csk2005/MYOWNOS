/* disk.h */
#pragma once
#include <stdio.h>
#include "osapi.h"
#include "omnistd.h"

#define DriveC      0x01
#define DriveD      0x02
#define Base        $1 "D:\\MYOWNOS\\drives\\disk."

#define Blocksize   512

typedef int8 block[512];

internal packed struct s_disk {
    int32 fd;
    int16 blocks;
    int8 drive:2;
};
typedef struct s_disk disk;

internal disk *dattach(int8);
internal void ddetach(disk*);

internal void dshow(disk*);
internal int16 openfiles(disk*);
internal void closeallfiles(disk*);

#define diskIO(func,dd,addr,block_no) ( \
    (dd) &&  \
    (lseek($i (dd)->fd, $i ((block_no-1) * Blocksize), SEEK_SET) != -1) && \
    ((func)($i (dd)->fd,$c (addr), Blocksize) == Blocksize) \
)
#define dread(dd,addr,block_no)     diskIO(read, dd, addr, block_no) 

#define dwrite(dd,addr,block_no)    diskIO(write, dd, addr, block_no) 