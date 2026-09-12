#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

extern Item *ItemInit(void *value, ItemFreeFunction *freeFunction,
                      ItemPrintFunction *printFunction)
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

// extern void ItemPrintInt(void *item)
// {
//     if (item != NULL)
//     {
//         printf("%s\n", (char *)item);
//     }
// }
