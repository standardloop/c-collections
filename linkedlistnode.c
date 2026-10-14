#include <limits.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>

#include "./collections.h"

extern LinkedListNode *LinkedListNodeInit(Item *item)
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

extern void AllLinkedListNodesFree(LinkedListNode *head)
{
    LinkedListNode *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        LinkedListNodeFree(temp);
    }
}

extern void LinkedListNodeFree(LinkedListNode *node)
{
    if (node != NULL)
    {
        ItemFree(node->item);
        free(node);
    }
}
