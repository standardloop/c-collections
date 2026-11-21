#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

const char SL_COLLECTIONS_TYPE_INT = 0;

extern void *DuplicateInt(void *original)
{
    if (original == NULL)
    {
        return NULL;
    }

    int *copy = malloc(sizeof(int));
    if (copy != NULL)
    {
        *copy = *(int *)original;
    }
    return copy;
}

extern void PrintInt(void *value)
{
    if (value != NULL)
    {
        printf("%d", *((int *)value));
    }
}

extern char *IntToString(void *num)
{
    int length = snprintf(NULL, 0, "%d", *(int *)num);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", *(int *)num);
    return str;
}

extern u_int32_t IntHash(void *integer)
{
    if (integer == NULL)
    {
        return 0;
    }
    return (u_int32_t) * (int *)integer;
}

extern bool IntEquivalence(void *int1, void *int2)
{
    if (int1 == NULL && int2 == NULL)
    {
        return true;
    }
    else if (int1 == NULL || int2 == NULL)
    {
        return false;
    }
    return *(int *)int1 == *(int *)int2;
}

ItemValueOperations ItemValueIntOperations = {.freeFunction = free,
                                              .printFunction = PrintInt,
                                              .toStringFunction = IntToString,
                                              .duplicateFunction = DuplicateInt,
                                              .equivalenceFunction =
                                                  IntEquivalence,
                                              .hashFunction = IntHash};
