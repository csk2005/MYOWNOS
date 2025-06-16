/* disk.h */
#pragma once
#include <stdio.h>
#include "osapi.h"

#define DriveC      0x01
#define DriveD      0x02
#define Base        "D:\\MYOWNOS\\drives\\disk."

internal struct packed s_disk {
    int32 fd;
    int16 blocks;
    bool attached:1;
    int8 drive:2;
};
typedef internal struct packed s_disk disk;

internal disk *dattach(int8);
internal void ddetach(disk*);
internal void dinit(void);