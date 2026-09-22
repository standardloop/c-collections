#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

const char SL_COLLECTIONS_TYPE_STRING = 0;
const char SL_COLLECTIONS_TYPE_INT = 0;
const char SL_COLLECTIONS_TYPE_LIST = 0;
const char SL_COLLECTIONS_TYPE_HASHMAP = 0;
const char SL_COLLECTIONS_TYPE_COMPLEX_HASHMAP = 0;

const char SL_COLLECTIONS_TYPE_LINKED_LIST = 0;

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
    if (list == NULL)
    {
        return 0;
    }
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

ItemValueOperations ItemValueStringOperations = {
    .toStringFunction = StringToString,
    .freeFunction = free,
    .printFunction = PrintString,
    .duplicateFunction = DuplicateString,
    .equivalenceFunction = StringEquivalence,
    .hashFunction = StringHash};

ItemValueOperations ItemValueIntOperations = {.toStringFunction = IntToString,
                                              .freeFunction = free,
                                              .printFunction = PrintInt,
                                              .duplicateFunction = DuplicateInt,
                                              .equivalenceFunction =
                                                  IntEquivalence,
                                              .hashFunction = IntHash};

ItemValueOperations ItemValueListOperations = {
    .toStringFunction = ListToString,
    .freeFunction = ListFree,
    .printFunction = ListPrint,
    .duplicateFunction = ListDuplicate,
    .equivalenceFunction = ListEquivalence,
    .hashFunction = ListHash};

ItemValueOperations ItemValueHashMapOperations = {
    .toStringFunction = HashMapToString,
    .freeFunction = HashMapFree,
    .printFunction = HashMapPrint,
    .duplicateFunction = HashMapDuplicate,
    .equivalenceFunction = HashMapEquivalence,
    .hashFunction = HashMapHash};

ItemValueOperations ItemValueComplexHashMapOperations = {
    .toStringFunction = ComplexHashMapToString,
    .freeFunction = ComplexHashMapFree,
    .printFunction = ComplexHashMapPrint,
    .duplicateFunction = ComplexHashMapDuplicate,
    .equivalenceFunction = ComplexHashMapEquivalence,
    .hashFunction = ComplexHashMapHash};

ItemValueOperations ItemValueLinkedListOperations = {
    .toStringFunction = LinkedListToString,
    .freeFunction = LinkedListFree,
    .printFunction = LinkedListPrint,
    .duplicateFunction = LinkedListDuplicate};
