#include <limits.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>

#include "./collections.h"

extern LinkedList *LinkedListInit()
{
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL)
    {
        return NULL;
    }
    list->size = 0;
    list->head = NULL;
    return list;
}

extern bool IsLinkedListEmpty(LinkedList *list)
{
    return (list->size == 0);
}

extern void LinkedListPrint(void *list)
{
    if (IsLinkedListEmpty(list) || list == NULL)
    {
        return;
    }

    printf("[");
    LinkedListNode *iterator = ((LinkedList *)list)->head;
    while (iterator != NULL)
    {
        ItemPrint(iterator->item);
        if (iterator->next != NULL)
        {
            printf(", ");
        }
        iterator = iterator->next;
    }
    printf("]");
    // printf("\n");
}

extern void LinkedListAddToFront(LinkedList *list, Item *item)
{

    if (list == NULL)
    {
        return;
    }
    LinkedListNode *list_node = LinkedListNodeInit(item);
    if (list_node == NULL)
    {
        return;
    }

    LinkedListNode *head = list->head;
    if (head == NULL)
    {
        list->head = list_node;
        list->size = 1;
        return;
    }
    list_node->next = head;
    list->head = list_node;
    list->size++;
    return;
}

extern void LinkedListAddToBack(LinkedList *list, Item *item)
{
    if (list == NULL)
    {
        return;
    }

    LinkedListNode *list_node = LinkedListNodeInit(item);
    if (list_node == NULL)
    {
        return;
    }

    LinkedListNode *head = list->head;
    if (head == NULL)
    {
        list->head = list_node;
        list->size = 1;
        return;
    }

    LinkedListNode *last = list->head;

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = list_node;
    list->size++;

    return;
}

extern void LinkedListFree(void *list)
{
    if (list != NULL)
    {
        if (((LinkedList *)list)->head != NULL)
        {
            AllLinkedListNodesFree(((LinkedList *)list)->head);
        }
        free(list);
    }
}

extern void LinkedListDeleteFront(LinkedList *list)
{
    LinkedListDeleteAtIndex(list, 0);
}

extern void LinkedListDeleteBack(LinkedList *list)
{
    LinkedListDeleteAtIndex(list, list->size - 1);
}

extern void LinkedListDeleteAtIndex(LinkedList *list, u_int64_t index)
{
    if (list == NULL || IsLinkedListEmpty(list) || index > list->size - 1)
    {
        return;
    }
    if (index == 0)
    {
        LinkedListNode *temp = list->head;
        list->head = list->head->next;
        LinkedListNodeFree(temp);
        list->size--;
        return;
    }
    LinkedListNode *index_node = list->head;
    LinkedListNode *index_node_prev = list->head;

    for (u_int64_t i = 0; i < index - 1; i++)
    {
        index_node_prev = index_node_prev->next;
    }

    for (u_int64_t i = 0; i < index; i++)
    {
        index_node = index_node->next;
    }
    index_node_prev->next = index_node->next;
    LinkedListNodeFree(index_node);
    list->size--;
    return;
}

extern void LinkedListAddAtIndex(LinkedList *list, Item *item, u_int64_t index)
{
    if (list == NULL || IsLinkedListEmpty(list) || index > list->size - 1)
    {
        return;
    }
    if (index == 0)
    {
        LinkedListAddToFront(list, item);
        return;
    }
    // LinkedListNode *index_node = list->head;
    LinkedListNode *index_node_prev = list->head;

    for (u_int64_t i = 0; i < index - 1; i++)
    {
        index_node_prev = index_node_prev->next;
    }

    // for (u_int64_t i = 0; i < index; i++)
    // {
    //     index_node = index_node->next;
    // }
    LinkedListNode *new = LinkedListNodeInit(item);
    if (new != NULL)
    {
        new->next = index_node_prev->next;
        index_node_prev->next = new;
        list->size++;
    }
    return;
}

extern char *LinkedListToString(void *list)
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
    LinkedListNode *iterator = ((LinkedList *)list)->head;
    while (iterator != NULL)
    {
        char *list_element = ItemToString(iterator->item);
        size_t list_element_len = strlen(list_element);

        needs_comma = iterator->next != NULL;

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
        iterator = iterator->next;
    }

    list_as_string[list_as_string_size - 2] = BRACKET_CLOSE_CHAR;
    list_as_string[list_as_string_size - 1] = NULL_CHAR;
    // printf("[JOSH]: %s\n", list_as_string);
    // printf("[JOSH]: %d\n", (int)strlen(list_as_string));
    return list_as_string;
}

extern void *LinkedListDuplicate(void *list)
{
    if (list == NULL)
    {
        return NULL;
    }
    LinkedList *list_ptr = (LinkedList *)list;
    LinkedList *dupe = LinkedListInit();

    LinkedListNode *list_ptr_itr = list_ptr->head;
    while (list_ptr_itr != NULL)
    {
        // I think this is O(n^2), fix this
        LinkedListAddToBack(dupe, ItemDuplicate(list_ptr_itr->item));
        list_ptr_itr = list_ptr_itr->next;
    }
    return dupe;
}

extern u_int32_t LinkedListHash(void *list)
{
    if (list == NULL)
    {
        return 0;
    }
    LinkedList *list_ptr = (LinkedList *)list;
    if (list_ptr->size == 0)
    {
        return 0;
    }
    u_int32_t sum = 0;
    LinkedListNode *iterator = list_ptr->head;
    while (iterator != NULL)
    {
        sum += ItemHash(iterator->item);
        iterator = iterator->next;
    }
    return sum;
}

extern bool LinkedListEquivalence(void *list1, void *list2)
{
    if (list1 == NULL && list2 == NULL)
    {
        return true;
    }

    if (list1 == NULL || list2 == NULL)
    {
        return false;
    }
    LinkedList *list1_ptr = (LinkedList *)list1;
    LinkedList *list2_ptr = (LinkedList *)list2;
    if (list1_ptr->size != list2_ptr->size)
    {
        return false;
    }
    LinkedListNode *iterator1 = list1_ptr->head;
    LinkedListNode *iterator2 = list1_ptr->head;
    while (iterator1 != NULL && iterator2 != NULL)
    {
        if (!ItemEquivalence(iterator1->item, iterator2->item))
        {
            return false;
        }
        iterator1 = iterator1->next;
        iterator2 = iterator2->next;
    }
    return true;
}
