#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

extern Item *ItemInit(void *value, ItemFreeFunction *freeFunction,
                      ItemPrintFunction *printFunction,
                      ItemToStringFunction *toStringFunction)
{
    if (value == NULL)
    {
        return NULL;
    }
    Item *this = malloc(sizeof(Item));
    this->value = value;
    if (freeFunction == NULL)
    {
        this->freeFunction = free;
    }
    else
    {
        this->freeFunction = freeFunction;
    }
    if (printFunction == NULL)
    {
        this->printFunction = NULL;
        // I guess this isn't mandatory?
    }
    else
    {
        this->printFunction = printFunction;
    }
    if (toStringFunction == NULL)
    {
        this->toStringFunction = DefaultToString;
    }
    else
    {
        this->toStringFunction = toStringFunction;
    }

    return this;
}

extern void ItemFree(Item *item)
{
    if (item != NULL && item->value != NULL && item->freeFunction != NULL)
    {
        item->freeFunction(item->value);
        free(item);
    }
}

extern char *DefaultToString(void *v)
{
    // v
    (void)v; // should be NULL
    char *null_str = QuickAllocatedString("null");
    return null_str;
}

// XD
extern char *StringToString(void *s)
{
    return (char *)(s);
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
    if (item == NULL || item->toStringFunction == NULL)
    {
        return NULL;
    }
    return item->toStringFunction(item->value);
}

extern void ItemPrint(Item *item)
{
    if (item != NULL && item->value != NULL && item->printFunction != NULL)
    {
        item->printFunction(item->value);
    }
}

extern void ItemPrintString(void *item)
{
    if (item != NULL)
    {
        printf("\"%s\"", (char *)item);
    }
}

extern void ItemPrintInt(void *item)
{
    if (item != NULL)
    {
        printf("%s\n", (char *)item);
    }
}
