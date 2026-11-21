#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

const char SL_COLLECTIONS_TYPE_NULL = 0;

extern void NULLPrint(void *v)
{
    (void)v;
    printf("null");
}

extern void *NULLDuplicate(void *v)
{
    (void)v; // should be NULL
    return NULL;
}

extern char *NULLToString(void *v)
{
    (void)v; // should be NULL
    char *null_str = QuickAllocatedString("null");
    return null_str;
}
