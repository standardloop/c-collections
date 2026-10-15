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

extern char *StackToString(void *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    // Stack *s_ptr = (Stack *)s;
    return NULL;
}

u_int32_t StackHash(void *s)
{
    if (s == NULL)
    {
        return 0;
    }

    Stack *s_ptr = (Stack *)s;
    if (s_ptr <= 0)
    {
        return 0;
    }
    return ListHash(s_ptr->list) + s_ptr->top;
}

bool StackEquivalence(void *s1, void *s2)
{
    if (s1 == NULL && s2 == NULL)
    {
        return true;
    }

    if (s1 == NULL || s2 == NULL)
    {
        return false;
    }

    Stack *s1_ptr = (Stack *)s1;
    Stack *s2_ptr = (Stack *)s2;
    if (s1_ptr->top != s2_ptr->top)
    {
        return false;
    }
    return ListEquivalence(s1_ptr->list, s2_ptr->list);
}

extern void *StackDuplicate(void *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    Stack *s_ptr = (Stack *)s;
    if (!IS_VALID_STACK(s_ptr))
    {
        return NULL;
    }
    Stack *dupe = malloc(sizeof(Stack));
    dupe->top = s_ptr->top;
    dupe->list = ListDuplicate(s_ptr->list);

    return dupe;
}
