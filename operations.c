#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

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

extern void *DuplicateString(void *original)
{
    return (void *)strdup((char *)original);
}

ItemValueOperations ItemValueStringOperations = {
    .toStringFunction = StringToString,
    .freeFunction = free,
    .printFunction = ItemPrintString,
    .duplicateFunction = DuplicateString};

ItemValueOperations ItemValueIntOperations = {.toStringFunction = IntToString,
                                              .freeFunction = free,
                                              .printFunction = ItemPrintInt,
                                              .duplicateFunction =
                                                  DuplicateInt};

ItemValueOperations ItemValueListOperations = {.toStringFunction = ListToString,
                                               .freeFunction = ListFree,
                                               .printFunction = ListPrint,
                                               .duplicateFunction =
                                                   ListDuplicate};

ItemValueOperations ItemValueHashMapOperations = {
    .toStringFunction = HashMapToString,
    .freeFunction = HashMapFree,
    .printFunction = HashMapPrint,
    .duplicateFunction = HashMapDuplicate};
