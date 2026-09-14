#include <assert.h>
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
    assert(strcmp(test_string, "[{\"key\":\"value\"}]") == 0);
    ListFree(list);
    free(test_string);
}

int main()
{
    testToString();
    TestItem();
    TestList();
    TestHashMap();
    sleep(1);
    return EXIT_SUCCESS;
}
