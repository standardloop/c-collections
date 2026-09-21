#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

const char TYPE_LIST = 0;
const char TYPE_HASHMAP = 0;
const char TYPE_LINKED_LIST = 0;
const char TYPE_STRING = 0;

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

#define FLOAT_CHAR_MAX 10
extern char *DoubleToString(void *num)
{
    char *double_as_string =
        malloc((sizeof(char) * FLOAT_CHAR_MAX) + sizeof(char));
    if (double_as_string == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    (void)gcvt(*(double *)num, FLOAT_CHAR_MAX, double_as_string);
    double_as_string[FLOAT_CHAR_MAX] = NULL_CHAR;
    return double_as_string;
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

extern u_int32_t ListHash(void *list)
{
    List *list_ptr = (List *)list;
    if (list_ptr->size == 0)
    {
        return 0;
    }
    u_int32_t sum = 0;

    for (u_int32_t i = 0; i < list_ptr->size; i++)
    {
        if (list_ptr->items[i] != NULL)
        {
            sum += ItemHash(list_ptr->items[i]);
        }
    }
    return sum;
}

ItemValueOperations ItemValueStringOperations = {
    .toStringFunction = StringToString,
    .freeFunction = free,
    .printFunction = PrintString,
    .duplicateFunction = DuplicateString,
    .hashFunction = NULL,
    .equivalenceFunction = NULL};

ItemValueOperations ItemValueIntOperations = {.toStringFunction = IntToString,
                                              .freeFunction = free,
                                              .printFunction = PrintInt,
                                              .duplicateFunction =
                                                  DuplicateInt};

ItemValueOperations ItemValueListOperations = {
    .toStringFunction = ListToString,
    .freeFunction = ListFree,
    .printFunction = ListPrint,
    .duplicateFunction = ListDuplicate,
    .hashFunction = ListHash,
    .equivalenceFunction = ListEquivalence};

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
