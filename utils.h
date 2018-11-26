#ifndef _DEFS_H
#define _DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a,b) (a < b ? a : b)
#define MAXSTR 100

void* checked_malloc(size_t);

#endif
