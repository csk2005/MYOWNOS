/* fs.h */
#pragma once
#include <stdio.h>
#include "osapi.h"
#include "disk.h"

#define Magic1          (0xdd05)
#define Magic2          (0xaa55)
#define InodesPerBlock  (16)
#define PtrPerInode     (8)
#define PtrPerBlock     (256)

typedef int16 ptr;
typedef int8 bootsector;

internal packed struct s_superblock{
    bootsector boot;
    int16 _;
    int16 blocks;
    int16 inodeblocks;
    int16 inodes;
    int16 magic1;
    int16 magic2;
};
typedef struct s_superblock superblock;

internal packed struct s_filesystem {
    int8 drive;
    disk* dd;
    bool *bitmap;
    superblock metadata;
};
typedef struct s_filesystem filesystem;

internal packed struct s_filename {
    int8 name[8];
    int8 ext[3];
}; 
typedef struct s_filename filename;

internal packed struct s_inode {
    struct {
        int8 _:4;
        int8 type:3;
        bool valid:1;
    } validtype;
    int16 size;
    filename name;
    ptr indirect;
    ptr direct[PtrPerInode];
};
typedef struct s_inode inode;

internal packed union u_fsblock{
    superblock super;
    int8 data[Blocksize];
    ptr pointers[PtrPerBlock];
    inode inodes[InodesPerBlock];
};
typedef union u_fsblock fsblock;

filesystem *fsformat(disk*, bootsector*);
 