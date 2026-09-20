#include <_string.h>
#include <assert.h>
#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testToString()
{
    HashMap *map = HashMapInitDefault();
    HashMapInsert(map, HashMapItemInit(QuickAllocatedString("key"),
                                       ItemInit(QuickAllocatedString("value"),
                                                &ItemValueStringOperations)));

    List *list = ListInitDefault();
    ListAddFirst(list, ItemInit(map, &ItemValueHashMapOperations));

    char *test_string = ListToString(list);
    TestCaseVerify(true, "check a complex structure to string",
                   strcmp(test_string, "[{\"key\":\"value\"}]") == 0);
    ListFree(list);
    free(test_string);
}

int main(void)
{
    TestingInit();
    testToString();
    TestItem();
    TestList();
    TestHashMap();
    TestLinkedList();
    TestingTearDown();
    sleep(1);
    return EXIT_SUCCESS;
}
