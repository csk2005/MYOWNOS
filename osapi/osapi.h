/* command.h */
#pragma once
#include <stdio.h>
#include <os.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define getposixfd(x) fds[(x)]
#define assert_initialized() if(!initialized) reterr(ErrInit)

private bool isopen(fd);
private void setupfds(void);
private void zero(int8*, int16);