/* command.h */
#pragma once
#include <stdio.h>
#include "os.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define getposixfd(x) fds[(x)]
#define assert_initialized() if(!initialized) reterr(ErrInit)

#ifdef Library
 private bool isopen(fd);
 private void setupfds(void);
 private int16 stringlen(int8*);
#endif

internal int8 *strnum(int8*,int8);
