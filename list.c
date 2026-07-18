#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <standardloop/util.h>

#include "./collections.h"

static inline bool isListFull(List *);
static inline bool isListEmpty(List *);
static void ListFreeItems(Item **, u_int32_t, bool);
static void listResize(List *);

extern List *DefaultListInit(void)
{
    return ListInit(DEFAULT_LIST_SIZE);
}

extern List *ListInit(u_int32_t initial_capacity)
{
    List *list = malloc(sizeof(List));
    if (list == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    list->items = malloc(sizeof(void *) * initial_capacity);
    if (list->items == NULL)
    {
        ListFree(list);
        errno = ENOMEM;
        return NULL;
    }
    return list;
}

extern void ListAddFirst(List *list, Item *item)
{
    ListAdd(list, item, 0);
}

extern void ListAddLast(List *list, Item *item)
{
    if (list == NULL || item == NULL)
    {
        return;
    }
    ListAdd(list, item, list->size);
}

extern void ListAdd(List *list, Item *item, u_int32_t index)
{
    if (list == NULL || item == NULL)
    {
        return;
    }
    if (isListFull(list))
    {
        listResize(list);
    }
    for (u_int32_t i = list->size; i > index; i--)
    {
        list->items[i] = list->items[i - 1];
    }

    list->items[index] = item;
    list->size++;
}

static void listResize(List *list)
{
    if (list == NULL)
    {
        return;
    }

    Item **newList = malloc(sizeof(Item *) * list->capacity * DEFAULT_LIST_RESIZE_MULTIPLE);
    if (newList == NULL)
    {
        // printf("Couldn't resize list, not enough memory!\n");
        errno = ENOMEM;
        return;
    }

    for (u_int32_t i = 0; i < list->size; i++)
    {
        newList[i] = list->items[i];
    }
    ListFreeItems(list->items, list->size, false);
    list->items = newList;
    list->capacity *= DEFAULT_LIST_RESIZE_MULTIPLE;
}

static inline bool isListFull(List *list)
{
    return list->capacity == list->size;
}

static inline bool isListEmpty(List *list)
{
    return list->size == 0;
}

static void ListFreeItems(Item **list, u_int32_t size, bool deep)
{
    if (list == NULL)
    {
        return;
    }
    if (deep)
    {
        for (u_int32_t i = 0; i < size; i++)
        {
            ItemFree(list[i]);
        }
    }
    free(list);
}

extern void ListFree(List *list)
{
    if (list == NULL)
    {
        return;
    }
    if (list->items != NULL)
    {
        ListFreeItems(list->items, list->size, true);
    }
    free(list);
}

extern void ListPrint(List *list)
{
    if (list == NULL)
    {
        return;
    }
    printf("[");
    for (u_int32_t i = 0; i < list->size; i++)
    {
        ItemPrint(list->items[i]);
        if (i != list->size - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

extern void ListRemove(List *list, u_int32_t index)
{
    if (list == NULL || index >= list->size || isListEmpty(list))
    {
        return;
    }
    ItemFree(list->items[index]);

    for (u_int32_t i = index + 1; i < list->size; i++)
    {
        list->items[i - 1] = list->items[i];
    }

    list->size--;
}

extern void ListRemoveFirst(List *list)
{
    if (list == NULL)
    {
        return;
    }
    ListRemove(list, 0);
}

extern void ListRemoveLastitem(List *list)
{
    if (list == NULL)
    {
        return;
    }
    ListRemove(list, list->size - 1);
}

// need to have another function pointer for this
// extern List *ListReplicate(List *list)
// {
//     if (list == NULL)
//     {
//         return NULL;
//     }
//     List *deep_clone = ListInit(list->capacity);

//     for (u_int32_t i = 0; i < list->size; i++)
//     {
//         deep_clone->items[i] = JSONValueReplicate(list->items[i]);
//         deep_clone->size++;
//     }
//     return deep_clone;
// }

extern Item *ListGetAtIndex(List *list, u_int32_t index)
{
    if (list == NULL || list->size < index)
    {
        return NULL;
    }
    return list->items[index];
}

extern void ListPrintInfo(List *list)
{
    if (list != NULL)
    {
        printf("list->size     = %d\n", list->size);
        printf("list->capacity = %d\n", list->capacity);
        printf("percent full   = %.2f%%\n", (float)list->size * 100 / list->capacity);
    }
}
