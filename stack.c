#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

#define DEFAULT_STACK_SIZE 16
#define DEFAULT_STACK_RESIZE_MULTIPLE 2

#define IS_VALID_STACK(s) ((s) != NULL && (s)->list != NULL)

extern Stack *StackInit(u_int32_t initial_capacity, u_int8_t resize_multiple)
{
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
    {
        return NULL;
    }
    s->list = ListInit(initial_capacity, resize_multiple);
    s->top = -1;
    return s;
}

extern Stack *StackInitDefault(void)
{
    return StackInit(DEFAULT_STACK_SIZE, DEFAULT_STACK_RESIZE_MULTIPLE);
}

extern void StackPush(Stack *s, Item *i)
{
    if (!IS_VALID_STACK(s))
    {
        return;
    }
    ListAddAtIndex(s->list, i, ++s->top);
}

extern Item *StackPop(Stack *s)
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

extern Item *StackPeek(Stack *s)
{
    if (!IS_VALID_STACK(s))
    {
        return NULL;
    }
    return s->list->items[s->top];
}

extern u_int32_t StackSize(Stack *s)
{
    if (!IS_VALID_STACK(s))
    {
        return 0;
    }
    assert(s->top == s->list->size - 1);
    return s->top + 1;
}

extern void StackPrint(void *s)
{
    if (s != NULL)
    {
        Stack *stack_ptr = (Stack *)s;
        if (stack_ptr->list != NULL)
        {
            printf("stack(");
            ListPrint(stack_ptr->list);
            printf(")");
        }
    }
}

extern void StackFree(void *s)
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
