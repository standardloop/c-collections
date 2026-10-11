#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

#define IS_VALID_STACK(s) ((s) != NULL && (s)->list != NULL)

Stack *StackInit()
{

    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
    {
        return NULL;
    }
    s->list = ListInitDefault();
    s->top = -1;
    return s;
}

void StackPush(Stack *s, Item *i)
{
    if (!IS_VALID_STACK(s))
    {
        return;
    }
    ListAddAtIndex(s->list, i, ++s->top);
}

Item *StackPop(Stack *s)
{
    if (!IS_VALID_STACK(s) || s->top < 0)
    {
        return NULL;
    }
    Item *ret = s->list->items[s->top];
    s->list->items[s->top] = NULL;
    s->top--;
    return ret;
}

Item *StackPeek(Stack *s)
{
    if (!IS_VALID_STACK(s))
    {
        return NULL;
    }
    return s->list->items[s->top];
}

u_int32_t StackSize(Stack *s)
{
    if (!IS_VALID_STACK(s))
    {
        return 0;
    }
    assert(s->top == s->list->size - 1);
    return s->top + 1;
}

void StackFree(void *s)
{
    if (s != NULL)
    {
        Stack *stack_ptr = (Stack *)s;
        if (stack_ptr->list != NULL)
        {
            ListFree(stack_ptr->list);
        }
        free(stack_ptr);
    }
}
