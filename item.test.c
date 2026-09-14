#include <assert.h>
#include <limits.h>
#include <standardloop/util.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./collections.h"

extern void TestItem()
{
    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    assert(item_1 != NULL);
    assert(strcmp(test_string_1, ItemToString(item_1)) == 0);
    ItemFree(item_1);

    int *test_int_1 = malloc(sizeof(int));
    *test_int_1 = 42;
    assert(test_int_1 != NULL);
    Item *item_2 = ItemInit(test_int_1, &ItemValueIntOperations);
    assert(item_2 != NULL);
    assert(strcmp("42", ItemToString(item_2)) == 0);
    ItemFree(item_2);
}
