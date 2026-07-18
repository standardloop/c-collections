#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <standardloop/util.h>

#include "./collections.h"

void printInt(void *value)
{
    if (value != NULL)
    {
        printf("%d", *(int *)value);
    }
}

int main()
{
    List *list = ListInit(1, 2);
    assert(list != NULL);
    assert(list->capacity == 1);

    char *test_string_1 = QuickAllocatedString("teststring1");
    assert(test_string_1 != NULL);
    Item *list_item_1 = ItemInit(test_string_1, free, ItemPrintString);
    assert(list_item_1 != NULL);

    ListAddFirst(list, list_item_1);
    assert(list->size == 1);

    char *test_string_2 = QuickAllocatedString("teststring2");
    assert(test_string_2 != NULL);
    Item *list_item_2 = ItemInit(test_string_2, free, ItemPrintString);
    assert(list_item_2 != NULL);
    ListAddFirst(list, list_item_2);

    ListPrint(list);
    ListPrintInfo(list);

    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    Item *list_item_3 = ItemInit(test_int_1, free, printInt);
    assert(list_item_3 != NULL);
    ListAddFirst(list, list_item_3);

    ListPrint(list);
    ListPrintInfo(list);

    ListFree(list);

    sleep(1);

    return EXIT_SUCCESS;
}
