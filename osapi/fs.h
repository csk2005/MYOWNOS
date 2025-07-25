/* fs.h */
#pragma once
#include <stdio.h>
#include "osapi.h"
#include "omnistd.h"
#include "disk.h"

#define Magic1          (0xdd05)
#define Magic2          (0xaa55)
#define InodesPerBlock  (16)
#define PtrPerInode     (8)
#define PtrPerBlock     (256)

typedef int16 ptr;
typedef int8 bootsector[500];
typedef bool bitmap;

internal packed enum {
    TypeNotValid = 0x00,
    TypeFile = 0x01,
    TypeDir = 0x03
};

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

internal struct s_filename {
    int8 name[8];
    int8 ext[3];
} packed; 
typedef struct s_filename filename;

internal struct s_inode {
    int8 validtype;
    int16 size;
    filename name;
    ptr indirect;
    ptr direct[PtrPerInode];
} packed;
typedef struct s_inode inode;

internal union u_fsblock{
    superblock super;
    int8 data[Blocksize];
    ptr pointers[PtrPerBlock];
    inode inodes[InodesPerBlock];
} packed;
typedef union u_fsblock fsblock;

internal filesystem *fsformat(disk*, bootsector*, bool);
internal bitmap *mkbitmap(filesystem*, bool);
internal int16 bitmapalloc(filesystem*, bitmap*);
internal void bitmapfree(filesystem*, bitmap*, int16);
internal void fsshow(filesystem*,bool);
internal inode *findinode(filesystem*, ptr);
internal int8 *file2str(filename *);

internal filesystem *fsmount(int8);
internal void fsunmount(filesystem *);