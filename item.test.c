#include <assert.h>
#include <limits.h>
#include <standardloop/util.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./collections.h"

static void testDuplicate()
{
    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *item = ItemInit(test_string_1, &ItemValueStringOperations);
    assert(item != NULL);

    Item *dupe = ItemDuplicate(item);
    assert(dupe != NULL);
    assert(strcmp(dupe->value, item->value) == 0);

    ItemFree(item);
    ItemFree(dupe);
}

static void testToStringSimple()
{
    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    assert(item_1 != NULL);
    char *item_value_as_string = ItemToString(item_1);
    assert(strcmp("\"teststring1\"", item_value_as_string) == 0);
    ItemFree(item_1);
    free(item_value_as_string);
}

extern void TestItem()
{
    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    assert(item_1 != NULL);
    assert(strcmp(test_string_1, item_1->value) == 0);
    ItemFree(item_1);

    int *test_int_1 = malloc(sizeof(int));
    *test_int_1 = 42;
    assert(test_int_1 != NULL);
    Item *item_2 = ItemInit(test_int_1, &ItemValueIntOperations);
    assert(item_2 != NULL);
    assert(42 == *((int *)item_2->value));
    ItemFree(item_2);

    testToStringSimple();
    testDuplicate();
}
