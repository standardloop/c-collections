#include <_string.h>
#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testQueueSimple()
{
    Queue *q = QueueInit();
    TestCaseVerify(true, "Ensure queue is not NULL", q != NULL);
    QueueEnqueue(q, ItemInit(strdup("test"), &SL_COLLECTIONS_TYPE_STRING,
                             &ItemValueStringOperations));
    TestCaseVerify(true, "Ensure QueueEnqueue incremented the queue size",
                   QueueSize(q) == 1);

    QueueEnqueue(q, ItemInit(strdup("test-again"), &SL_COLLECTIONS_TYPE_STRING,
                             &ItemValueStringOperations));
    TestCaseVerify(true, "Ensure QueueEnqueue incremented the queue size",
                   QueueSize(q) == 2);
    // QueuePrint(q);

    Item *i = QueueDequeue(q);
    TestCaseVerify(true, "Ensure QueueDequeue item is not NULL", i != NULL);
    TestCaseVerify(true, "Ensure QueueDequeue decremented the queue size",
                   QueueSize(q) == 1);
    TestCaseVerify(true, "Ensure QueueDequeue returned the correct item",
                   strcmp(i->value, "test") == 0);

    // QueuePrint(q);
    QueueFree(q);
}

static void testQueueToString() {}

static void testQueuePop() {}

static void testQueuePeek() {}

static void testQueueDuplicate() {}

extern void TestQueue()
{
    testQueueSimple();
    testQueueToString();
    testQueuePop();
    testQueuePeek();
    testQueueDuplicate();
}
