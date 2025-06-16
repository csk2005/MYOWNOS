/* command.h */
#pragma once
#include <stdio.h>
#include <os.h>
#include <sys/stat.h>
#include <fcntl.h>

#define getposixfd(x) fds[(x)]

private bool isopen(fd);