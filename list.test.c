#include <assert.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testListInList()
{

    List *list_inner = ListInitDefault();
    assert(list_inner != NULL);
    List *list_outer = ListInitDefault();
    assert(list_outer != NULL);
    ListAddFirst(list_outer,
                 ItemInit(list_inner, ListFree, ListPrint, ListToString));
    ListFree(list_outer);
}

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

static void testListToStringListWithinListSimple()
{
    List *list_inner = ListInitDefault();
    assert(list_inner != NULL);
    List *list_outer = ListInitDefault();
    assert(list_outer != NULL);
    ListAddFirst(list_outer,
                 ItemInit(list_inner, ListFree, ListPrint, ListToString));
    assert(list_outer->size == 1);
    // ListPrint(list_holder);
    char *list_outer_as_string = ListToString(list_outer);
    // printf("%s\n", list_outer_as_string);

    assert(list_outer->size == 1);
    assert(list_inner->size == 0);
    assert(strcmp(list_outer_as_string, "[[]]") == 0);

    ListFree(list_outer);
    free(list_outer_as_string);
}

static void testListToStringListWithinList()
{
    List *list_1 = ListInitDefault();
    assert(list_1 != NULL);
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring1"), free,
                                  ItemPrintString, StringToString));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring2"), free,
                                  ItemPrintString, StringToString));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring3"), free,
                                  ItemPrintString, StringToString));
    List *list_holder = ListInitDefault();
    assert(list_holder != NULL);
    ListAddFirst(list_holder,
                 ItemInit(list_1, ListFree, ListPrint, ListToString));

    char *nested = ListToString(list_holder);

    ListFree(list_holder);
    free(nested);
}

static void testListToString()
{
    List *list_1 = ListInitDefault();
    assert(list_1 != NULL);

    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring1"), free,
                                  ItemPrintString, StringToString));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring2"), free,
                                  ItemPrintString, StringToString));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring3"), free,
                                  ItemPrintString, StringToString));

    char *list_as_string = ListToString(list_1);
    ListFree(list_1);
    free(list_as_string);

    testListToStringListWithinList();
    testListToStringListWithinListSimple();
}

extern void TestList()
{
    testListToString();
    testListInList();
    testListPop();
}
