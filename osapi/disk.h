/* disk.h */
#pragma once
#include <stdio.h>
#include "osapi.h"

#define DriveC      0x01
#define DriveD      0x02
#define Base        "D:\\MYOWNOS\\drives\\disk."

internal packed struct s_disk {
    int32 fd;
    int16 blocks;
    int8 drive:2;
};
typedef struct s_disk disk;

internal disk *dattach(int8);
internal void ddetach(disk*);
internal void dinit(void);