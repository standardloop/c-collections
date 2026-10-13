#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

static bool isQueueEmpty(Queue *q);

extern Queue *QueueInit()
{
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL)
    {
        return NULL;
    }

    q->size = 0;
    q->head = NULL;
    q->tail = NULL;

    return q;
}

extern void QueuePrint(void *q)
{
    if (q != NULL)
    {
        printf("queue([");
        Queue *q_ptr = (Queue *)q;
        LinkedListNode *iter = q_ptr->head;
        LinkedListNode *iter_next = NULL;
        while (iter != NULL)
        {
            iter_next = iter->next;
            ItemPrint(iter->item);
            if (iter_next != NULL)
            {
                printf(", ");
            }

            iter = iter_next;
        }
        printf("])");
    }
}

extern void QueueFree(void *q)
{
    if (q != NULL)
    {
        Queue *q_ptr = (Queue *)q;
        while (q_ptr->head != NULL)
        {
            LinkedListNode *tmp = q_ptr->head;
            q_ptr->head = q_ptr->head->next;
            ItemFree(tmp->item);
            free(tmp);
        }

        if (q_ptr->tail != NULL)
        {
            free(q_ptr->tail);
        }
        free(q_ptr);
    }
}

extern void QueueEnqueue(Queue *q, Item *i)
{
    LinkedListNode *node = malloc(sizeof(LinkedListNode));

    if (node == NULL)
    {
        return;
    }

    node->item = i;
    node->next = NULL;

    if (q->head == NULL)
    {
        q->head = node;
        q->tail = node;
        q->size = 1;

        return;
    }

    q->tail->next = node;
    q->tail = node;
    q->size += 1;

    return;
}

static bool isQueueEmpty(Queue *q)
{
    return q->size == 0;
}

extern Item *QueueDequeue(Queue *q)
{
    if (isQueueEmpty(q))
    {
        return NULL;
    }

    LinkedListNode *tmp = NULL;

    Item *i = q->head->item;
    tmp = q->head;
    q->head = q->head->next;
    q->size -= 1;

    free(tmp);

    return i;
}

extern u_int64_t QueueSize(Queue *q)
{
    return q->size;
}
