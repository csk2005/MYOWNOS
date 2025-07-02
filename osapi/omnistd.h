/* omnistd.h */
#pragma once
#include <stdio.h>
#include "osapi.h"

#define copy(dst, src, len) memorycopy(dst, src, len, false)
#define strcopy(dst, src, len) memorycopy(dst, src, len, true)
#define getbit_(bit, pos) ((bit >> pos) & (1))
#define setbit_(bit, pos, value) (bit=(value ? (bit | (1 << pos)) : (bit & ~(1 << pos))))

internal void zero(int8*, int16);
internal void memorycopy(int8*, int8*, int16, bool);
internal int16 stringlen(int8*);

internal bool getbit(int8*, int16);
internal void setbit(int8*, int16, bool);