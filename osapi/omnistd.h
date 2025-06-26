/* omnistd.h */
#pragma once
#include <stdio.h>
#include "osapi.h"

#define copy(dst, src, len) memorycopy(dst, src, len, false)
#define strcopy(dst, src, len) memorycopy(dst, src, len, true)

internal void zero(int8*, int16);
internal void memorycopy(int8*, int8*, int16, bool);