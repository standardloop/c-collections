#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testListInList()
{

    List *list_inner = ListInitDefault();
    TestCaseVerify(true, "", list_inner != NULL);
    List *list_outer = ListInitDefault();
    TestCaseVerify(true, "", list_outer != NULL);
    ListAddFirst(list_outer, ItemInit(list_inner, &ItemValueListOperations));
    ListFree(list_outer);
}

static void testListPop()
{
    List *list = ListInitDefault();
    TestCaseVerify(true, "", list != NULL);

    char *test_string_1 = QuickAllocatedString("teststring1");
    TestCaseVerify(true, "", test_string_1 != NULL);
    Item *list_item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    TestCaseVerify(true, "", list_item_1 != NULL);

    ListAddFirst(list, list_item_1);
    char *assert_value_str_1 = list->items[0]->value;
    TestCaseVerify(true, "", strcmp(assert_value_str_1, "teststring1") == 0);
    TestCaseVerify(true, "", list->size == 1);

    Item *list_item_1_diff_ref = ListPopFirst(list);
    TestCaseVerify(true, "", list_item_1_diff_ref != NULL);
    TestCaseVerify(true, "", list->size == 0);

    TestCaseVerify(true, "",
                   strcmp(list_item_1_diff_ref->value, "teststring1") == 0);

    ItemFree(list_item_1_diff_ref);
    // ListPrint(list);
    // ListPrintInfo(list);
    ListFree(list);
}

static void testListToStringListWithinListSimple()
{
    List *list_inner = ListInitDefault();
    TestCaseVerify(true, "", list_inner != NULL);
    List *list_outer = ListInitDefault();
    TestCaseVerify(true, "", list_outer != NULL);
    ListAddFirst(list_outer, ItemInit(list_inner, &ItemValueListOperations));
    TestCaseVerify(true, "", list_outer->size == 1);
    // ListPrint(list_holder);
    char *list_outer_as_string = ListToString(list_outer);
    // printf("%s\n", list_outer_as_string);

    TestCaseVerify(true, "", list_outer->size == 1);
    TestCaseVerify(true, "", list_inner->size == 0);
    TestCaseVerify(true, "", strcmp(list_outer_as_string, "[[]]") == 0);

    ListFree(list_outer);
    free(list_outer_as_string);
}

static void testListToStringListWithinList()
{
    List *list_1 = ListInitDefault();
    TestCaseVerify(true, "", list_1 != NULL);
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring1"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring2"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring3"),
                                  &ItemValueStringOperations));
    List *list_holder = ListInitDefault();
    TestCaseVerify(true, "", list_holder != NULL);
    ListAddFirst(list_holder, ItemInit(list_1, &ItemValueListOperations));

    char *nested = ListToString(list_holder);

    ListFree(list_holder);
    free(nested);
}

static void testListToString()
{
    List *list_1 = ListInitDefault();
    TestCaseVerify(true, "", list_1 != NULL);

    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring1"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring2"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring3"),
                                  &ItemValueStringOperations));

    char *list_as_string = ListToString(list_1);
    ListFree(list_1);
    free(list_as_string);

    testListToStringListWithinList();
    testListToStringListWithinListSimple();
}

static void testListDuplicate()
{
    List *list_1 = ListInitDefault();
    TestCaseVerify(true, "", list_1 != NULL);

    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring1"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring2"),
                                  &ItemValueStringOperations));
    ListAddFirst(list_1, ItemInit(QuickAllocatedString("teststring3"),
                                  &ItemValueStringOperations));

    List *dupe = ListDuplicate(list_1);
    TestCaseVerify(true, "", dupe != NULL);
    TestCaseVerify(true, "", dupe->size == list_1->size);

    TestCaseVerify(
        true, "",
        strcmp(ListGetFirst(list_1)->value, ListGetFirst(dupe)->value) == 0);

    ListFree(list_1);
    ListFree(dupe);
}

extern void TestList()
{
    testListToString();
    testListInList();
    testListPop();
    testListDuplicate();
}
