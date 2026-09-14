#include <assert.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testListPop()
{
    List *list = ListInitDefault();
    assert(list != NULL);

    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *list_item_1 = ItemInit(test_string_1, free, ItemPrintString, NULL);
    assert(list_item_1 != NULL);

    ListAddFirst(list, list_item_1);
    char *assert_value_str_1 = list->items[0]->value;
    assert(strcmp(assert_value_str_1, "teststring1") == 0);
    assert(list->size == 1);

    Item *list_item_1_diff_ref = ListPopFirst(list);
    assert(list_item_1_diff_ref != NULL);
    assert(list->size == 0);

    assert(strcmp(list_item_1_diff_ref->value, "teststring1") == 0);

    ItemFree(list_item_1_diff_ref);
    // ListPrint(list);
    // ListPrintInfo(list);
    ListFree(list);
}

static void testListToString()
{
    List *list = ListInitDefault();
    assert(list != NULL);

    ListAddFirst(list, ItemInit(QuickAllocatedString("teststring1"), free,
                                ItemPrintString, StringToString));
    ListAddFirst(list, ItemInit(QuickAllocatedString("teststring2"), free,
                                ItemPrintString, StringToString));
    ListAddFirst(list, ItemInit(QuickAllocatedString("teststring3"), free,
                                ItemPrintString, StringToString));

    char *list_as_string = ListToString(list);
    ListFree(list);
    free(list_as_string);
}

extern void TestList()
{
    testListToString();

    List *list = ListInit(1, 2);
    assert(list != NULL);
    assert(list->capacity == 1);

    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *list_item_1 = ItemInit(test_string_1, free, ItemPrintString, NULL);
    assert(list_item_1 != NULL);

    ListAddFirst(list, list_item_1);
    char *assert_value_str_1 = list->items[0]->value;
    assert(strcmp(assert_value_str_1, "teststring1") == 0);
    assert(list->size == 1);

    char *test_string_2 = QuickAllocatedString("teststring2");
    assert(test_string_2 != NULL);
    Item *list_item_2 = ItemInit(test_string_2, free, ItemPrintString, NULL);
    assert(list_item_2 != NULL);
    ListAddFirst(list, list_item_2);
    char *assert_value_str_2 = list->items[0]->value;
    assert(strcmp(assert_value_str_2, "teststring2") == 0);

    // ListPrint(list);
    // ListPrintInfo(list);

    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    Item *list_item_3 = ItemInit(test_int_1, free, ItemPrintInt, NULL);
    assert(list_item_3 != NULL);
    ListAddFirst(list, list_item_3);
    int *assert_value = list->items[0]->value;
    assert(*(int *)assert_value == 100);

    ListFree(list);

    testListPop();
    // ListPrint(list);
    // ListPrintInfo(list);
}
