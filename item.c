#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

extern Item *ItemInit(void *value, ItemValueOperations *value_ops)
{
    if (value == NULL)
    {
        return NULL;
    }
    Item *this = malloc(sizeof(Item));
    this->value = value;
    if (value_ops->freeFunction == NULL)
    {
        value_ops->freeFunction = free;
    }

    if (value_ops->printFunction == NULL)
    {
        value_ops->printFunction = NULL;
        // I guess this isn't mandatory?
    }

    if (value_ops->toStringFunction == NULL)
    {
        value_ops->toStringFunction = DefaultToString;
    }
    if (value_ops->replicateFunction == NULL)
    {
        value_ops->replicateFunction = DefaultReplicate;
    }

    this->value_ops = value_ops;
    return this;
}

extern void ItemFree(Item *item)
{
    if (item != NULL)
    {
        if (item->value != NULL)
        {
            item->value_ops->freeFunction(item->value);
        }
        free(item);
    }
}

extern void *DefaultReplicate(void *v)
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

// XD
// actually, maybe we can use PutQuotesAroundString() here
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

extern char *ItemToString(Item *item)
{
    assert(item->value_ops->toStringFunction != NULL);
    if (item == NULL)
    {
        return NULL;
    }
    return item->value_ops->toStringFunction(item->value);
}

extern void ItemPrint(Item *item)
{
    if (item != NULL && item->value != NULL &&
        item->value_ops->printFunction != NULL)
    {
        item->value_ops->printFunction(item->value);
    }
}

extern void ItemPrintString(void *value)
{
    if (value != NULL)
    {
        printf("\"%s\"", (char *)value);
    }
}

extern void ItemPrintInt(void *value)
{
    if (value != NULL)
    {
        printf("%d\n", *((int *)value));
    }
}
