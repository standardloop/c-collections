#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>
#include <sys/_types/_u_int8_t.h>

#include "./collections.h"

#define DEFAULT_LIST_SIZE 16
#define DEFAULT_LIST_RESIZE_MULTIPLE 2

static inline bool isListFull(List *);
static inline bool isListEmpty(List *);
static void ListFreeItems(Item **, u_int32_t, bool);
static void listResize(List *);

extern List *ListInitDefault(void)
{
    return ListInit(DEFAULT_LIST_SIZE, DEFAULT_LIST_RESIZE_MULTIPLE);
}

extern List *ListInit(u_int32_t initial_capacity, u_int8_t resize_multiple)
{
    if (initial_capacity == 0)
    {
        errno = EINVAL;
        return NULL;
    }
    if (resize_multiple == 0 || resize_multiple == 1)
    {
        errno = EINVAL;
        return NULL;
    }

    List *list = malloc(sizeof(List));
    if (list == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    list->resize_multiple = resize_multiple;
    list->items = malloc(sizeof(Item *) * initial_capacity);
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
    if (list != NULL && item != NULL)
    {
        ListAddAtIndex(list, item, 0);
    }
}

extern void ListAddLast(List *list, Item *item)
{
    if (list != NULL && item != NULL)
    {
        ListAddAtIndex(list, item, list->size);
    }
}

extern void ListAddAtIndex(List *list, Item *item, u_int32_t index)
{
    if (list != NULL && item != NULL)
    {
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
}

static void listResize(List *list)
{
    if (list != NULL)
    {
        Item **newList =
            malloc(sizeof(Item *) * list->capacity * list->resize_multiple);
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
        list->capacity *= list->resize_multiple;
    }
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
    if (list != NULL)
    {
        if (deep)
        {
            for (u_int32_t i = 0; i < size; i++)
            {
                ItemFree(list[i]);
            }
        }
        free(list);
    }
}

extern void ListFree(void *list)
{
    if (list != NULL)
    {
        if (((List *)list)->items != NULL)
        {
            ListFreeItems(((List *)list)->items, ((List *)list)->size, true);
        }
        free(list);
    }
}

extern void ListPrint(void *list)
{
    if (list != NULL)
    {
        printf("[");
        for (u_int32_t i = 0; i < ((List *)list)->size; i++)
        {
            ItemPrint(((List *)list)->items[i]);
            if (i != ((List *)list)->size - 1)
            {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

extern void ListRemoveAtIndex(List *list, u_int32_t index, bool free)
{
    if (list != NULL && index < list->size && !isListEmpty(list))
    {
        if (free)
        {
            ItemFree(list->items[index]);
        }

        for (u_int32_t i = index + 1; i < list->size; i++)
        {
            list->items[i - 1] = list->items[i];
        }

        list->size--;
    }
}

extern void ListRemoveFirst(List *list)
{
    if (list != NULL)
    {
        ListRemoveAtIndex(list, 0, true);
    }
}

extern void ListRemoveLastitem(List *list)
{
    if (list != NULL)
    {
        ListRemoveAtIndex(list, list->size - 1, true);
    }
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

extern Item *ListGetFirst(List *list)
{
    return ListGetAtIndex(list, 0);
}

extern Item *ListGetAtIndex(List *list, u_int32_t index)
{
    if (list == NULL || list->size < index)
    {
        return NULL;
    }
    return list->items[index];
}

extern Item *ListPopAtIndex(List *list, u_int32_t index)
{
    if (list == NULL || list->size < index)
    {
        return NULL;
    }

    Item *return_val = list->items[index];
    ListRemoveAtIndex(list, index, false);
    return return_val;
}

extern Item *ListPopFirst(List *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    return ListPopAtIndex(list, 0);
}

extern void ListPrintInfo(List *list)
{
    if (list != NULL)
    {
        printf("list->size     = %d\n", list->size);
        printf("list->capacity = %d\n", list->capacity);
        printf("percent full   = %.2f%%\n\n",
               (float)list->size * 100 / list->capacity);
    }
}

extern char *ListToString(void *list)
{
    if (list == NULL)
    {
        return NULL;
    }
    size_t list_as_string_size = 3; // "[]\0"
    char *list_as_string = malloc(sizeof(char) * list_as_string_size);
    list_as_string[0] = BRACKET_OPEN_CHAR;
    list_as_string[1] = NULL_CHAR;

    size_t chars_written = 2 - 1;

    bool needs_comma = false;
    for (u_int64_t i = 0; i < ((List *)list)->size; i++)
    {
        char *list_element = ItemToString(((List *)list)->items[i]);
        size_t list_element_len = strlen(list_element);
        if (i < ((List *)list)->size - 1)
        {
            needs_comma = true;
        }
        list_as_string_size += list_element_len;
        list_as_string_size += needs_comma;
        list_as_string = realloc(list_as_string, list_as_string_size);

        CopyStringCanary(list_as_string, list_element, chars_written);
        chars_written += list_element_len;
        if (needs_comma)
        {
            CopyStringCanary(list_as_string, ",", chars_written);
            chars_written++;
        }
        needs_comma = false;
        free(list_element);
    }

    list_as_string[list_as_string_size - 2] = BRACKET_CLOSE_CHAR;
    list_as_string[list_as_string_size - 1] = NULL_CHAR;
    // printf("[JOSH]: %s\n", list_as_string);
    // printf("[JOSH]: %d\n", (int)strlen(list_as_string));
    return list_as_string;
}

extern void *ListDuplicate(void *list)
{
    if (list == NULL)
    {
        return NULL;
    }
    List *dupe =
        ListInit(((List *)list)->capacity, ((List *)list)->resize_multiple);
    if (dupe == NULL)
    {
        return NULL;
    }
    u_int8_t list_size = ((List *)list)->size;
    for (u_int32_t i = 0; i < list_size; i++)
    {
        dupe->items[i] = ItemDuplicate(((List *)list)->items[i]);
        // NULL check here?
        dupe->size++;
    }
    return dupe;
}
