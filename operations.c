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

extern void PrintString(void *value)
{
    if (value != NULL)
    {
        printf("\"%s\"", (char *)value);
    }
}

extern void PrintInt(void *value)
{
    if (value != NULL)
    {
        printf("%d", *((int *)value));
    }
}

extern char *StringToString(void *s)
{
    return (char *)PutQuotesAroundString(s, false);
}

extern char *IntToString(void *num)
{
    int length = snprintf(NULL, 0, "%d", *(int *)num);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", *(int *)num);
    return str;
}

extern void DefaultPrint(void *v)
{
    (void)v;
    printf("null");
}

extern void *DefaultDuplicate(void *v)
{
    (void)v; // should be NULL
    return NULL;
}

extern char *DefaultToString(void *v)
{
    // v
    (void)v; // should be NULL
    char *null_str = QuickAllocatedString("null");
    return null_str;
}

ItemValueOperations ItemValueStringOperations = {
    .toStringFunction = StringToString,
    .freeFunction = free,
    .printFunction = PrintString,
    .duplicateFunction = DuplicateString};

ItemValueOperations ItemValueIntOperations = {.toStringFunction = IntToString,
                                              .freeFunction = free,
                                              .printFunction = PrintInt,
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

ItemValueOperations ItemValueLinkedListOperations = {
    .toStringFunction = LinkedListToString,
    .freeFunction = LinkedListFree,
    .printFunction = LinkedListPrint,
    .duplicateFunction = LinkedListDuplicate};
