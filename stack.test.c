#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testStackSimple()
{
    Stack *s = StackInit();
    TestCaseVerify(true, "ensure StackInit returns a non NULL ptr", s != NULL);
    TestCaseVerify(true, "ensure the stacks underlying list is not NULL",
                   s->list != NULL);
    TestCaseVerify(true, "ensure the stacks top is initialized to -1",
                   s->top == -1);

    // ---
    StackPush(s, ItemInitV2(strdup("test"), &SL_COLLECTIONS_TYPE_STRING,
                            &ItemValueStringOperations));
    TestCaseVerify(true, "ensure the stack size is correct after pushing",
                   StackSize(s) == 1);

    TestCaseVerify(true, "ensure peek works",
                   strcmp(StackPeek(s)->value, "test") == 0);

    Item *top = StackPop(s);
    TestCaseVerify(true, "ensure pop works", strcmp(top->value, "test") == 0);

    ItemFree(top);
    StackFree(s);
}

static void testStackToString() {}
static void testStackPop() {}
static void testStackPeek() {}
static void testStackDuplicate() {}

extern void TestStack()
{
    testStackSimple();
    testStackToString();
    testStackPop();
    testStackPeek();
    testStackDuplicate();
}
