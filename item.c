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
        // I guess this isn't mandatory?
        value_ops->printFunction = DefaultPrint;
    }

    if (value_ops->toStringFunction == NULL)
    {
        value_ops->toStringFunction = DefaultToString;
    }
    if (value_ops->duplicateFunction == NULL)
    {
        value_ops->duplicateFunction = DefaultDuplicate;
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

extern Item *ItemDuplicate(Item *item)
{
    if (item == NULL)
    {
        return NULL;
    }
    return ItemInit(item->value_ops->duplicateFunction(item->value),
                    item->value_ops);
}
