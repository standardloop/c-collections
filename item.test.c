#include <_string.h>
#include <limits.h>
#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./collections.h"

static void testDuplicate()
{
    char *test_string_1 = strdup("teststring1");
    TestCaseVerify(true, "ensure test string is not NULL",
                   test_string_1 != NULL);
    Item *item = ItemInit(test_string_1, &ItemValueStringOperations);
    TestCaseVerify(true, "ensure ItemInit returns a non NULL pointer",
                   item != NULL);

    Item *dupe = ItemDuplicate(item);
    TestCaseVerify(true, "ensure ItemDuplicate returns a non NULL pointer",
                   dupe != NULL);
    TestCaseVerify(true, "",
                   dupe != NULL && item != NULL &&
                       strcmp(dupe->value, item->value) == 0);

    ItemFree(item);
    ItemFree(dupe);
}

static void testToStringSimple()
{
    char *test_string_1 = strdup("teststring1");
    TestCaseVerify(true, "Ensure test string is not NULL",
                   test_string_1 != NULL);

    Item *item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    TestCaseVerify(true, "Ensure ItemInit returns a non NULL value",
                   item_1 != NULL);

    char *item_value_as_string = ItemToString(item_1);
    TestCaseVerify(true, "ensure ItemToString returns a non NULL value",
                   item_value_as_string != NULL);
    TestCaseVerify(true, "ensure ItemToString matches the expected string",
                   strcmp("\"teststring1\"", item_value_as_string) == 0);
    ItemFree(item_1);
    free(item_value_as_string);
}

extern void TestItem()
{
    char *test_string_1 = strdup("teststring1");
    TestCaseVerify(true, "Ensure test string is not NULL",
                   test_string_1 != NULL);
    Item *item_1 = ItemInit(test_string_1, &ItemValueStringOperations);
    TestCaseVerify(true, "Ensure ItemInit doesn't return NULL", item_1 != NULL);
    TestCaseVerify(true, "Ensure item value matches the test string",
                   item_1 != NULL && strcmp(test_string_1, item_1->value) == 0);
    ItemFree(item_1);

    int *test_int_1 = malloc(sizeof(int));
    *test_int_1 = 42;
    TestCaseVerify(true, "Ensure test int is not NULL", test_int_1 != NULL);
    Item *item_2 = ItemInit(test_int_1, &ItemValueIntOperations);
    TestCaseVerify(true, "Ensure ItemInit returns non NULL", item_2 != NULL);
    TestCaseVerify(true, "Ensure item value matches the expected int value",
                   item_2 != NULL && 42 == *((int *)item_2->value));
    ItemFree(item_2);

    testToStringSimple();
    testDuplicate();
}
