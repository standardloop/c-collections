#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <standardloop/util.h>

#include "./collections.h"

extern Item *ItemInit(void *value, ItemFreeFunction *freeFunction, ItemPrintFunction *printFunction)
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

extern void ItemFree(Item *this)
{
    if (this != NULL && this->value != NULL && this->freeFunction != NULL)
    {
        this->freeFunction(this->value);
        free(this);
    }
}

extern void ItemPrint(Item *this)
{
    if (this != NULL && this->value != NULL && this->printFunction != NULL)
    {
        this->printFunction(this->value);
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
