
#include "./collections.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_u_int8_t.h>

static LinkedListNode *linkedListNodeInit(Item *item);
static void allLinkedListNodesFree(LinkedListNode *);
static void linkedListNodeFree(LinkedListNode *);

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

static LinkedListNode *linkedListNodeInit(Item *item)
{
    LinkedListNode *list_node = malloc(sizeof(LinkedListNode));
    if (list_node == NULL)
    {
        return NULL;
    }
    list_node->next = NULL;
    list_node->item = item;

    return list_node;
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

    LinkedListNode *iterator = ((LinkedList *)list)->head;
    while (iterator != NULL)
    {
        ItemPrint(iterator->item);
        // printf("%d -> ", iterator->value);
        iterator = iterator->next;
    }
    printf("\n");
}

extern void LinkedListAddToFront(LinkedList *list, Item *item)
{

    if (list == NULL)
    {
        return;
    }
    LinkedListNode *list_node = linkedListNodeInit(item);
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

    LinkedListNode *list_node = linkedListNodeInit(item);
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
            allLinkedListNodesFree(((LinkedList *)list)->head);
        }
        free(list);
    }
}

static void allLinkedListNodesFree(LinkedListNode *head)
{
    LinkedListNode *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        linkedListNodeFree(temp);
    }
}

static void linkedListNodeFree(LinkedListNode *list_node)
{
    if (list_node != NULL)
    {
        ItemFree(list_node->item);
        free(list_node);
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
        linkedListNodeFree(temp);
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
    linkedListNodeFree(index_node);
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
    LinkedListNode *new = linkedListNodeInit(item);
    if (new != NULL)
    {
        new->next = index_node_prev->next;
        index_node_prev->next = new;
        list->size++;
    }
    return;
}
