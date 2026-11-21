#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

extern void *DuplicateString(void *original)
{
    return (void *)strdup((char *)original);
}

extern void PrintString(void *value)
{
    if (value != NULL)
    {
        printf("\"%s\"", (char *)value);
    }
}

extern bool StringEquivalence(void *s1, void *s2)
{
    if (s1 == NULL && s2 == NULL)
    {
        return true;
    }
    if (s1 == NULL || s2 == NULL)
    {
        return false;
    }
    return strcmp(s1, s2) == 0;
}

extern char *StringToString(void *s)
{
    return (char *)PutQuotesAroundString(s, false);
}

// Jenkins's one_at_a_time
extern u_int32_t StringHash(void *str)
{
    if (str == NULL)
    {
        return 0;
    }
    char *key = (char *)str;
    u_int32_t len = strlen(key);
    u_int32_t hash = 0;

    for (u_int32_t i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

const char SL_COLLECTIONS_TYPE_STRING = 0;

ItemValueOperations ItemValueStringOperations = {
    .freeFunction = free,
    .printFunction = PrintString,
    .toStringFunction = StringToString,
    .duplicateFunction = DuplicateString,
    .equivalenceFunction = StringEquivalence,
    .hashFunction = StringHash};
