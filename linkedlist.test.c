#include "./collections.h"
#include <standardloop/testing.h>
#include <standardloop/util.h>

static void basicTest()
{
    LinkedList *list = LinkedListInit();
    TestCaseVerify(true, "ensure LinkedListInit doesn't return NULL",
                   list != NULL);
    TestCaseVerify(true, "ensure linked list is empty after creation",
                   IsLinkedListEmpty(list));

    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring1"),
                                        &ItemValueStringOperations));

    TestCaseVerify(true, "After adding one item, ensure size is 1",
                   list->size == 1);

    TestCaseVerify(true, "ensure the item we added has the correct value",
                   strcmp(list->head->item->value, "teststring1") == 0);

    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring2"),
                                        &ItemValueStringOperations));

    TestCaseVerify(true, "ensure another item added has the correct value",
                   strcmp(list->head->item->value, "teststring2") == 0);

    LinkedListAddToBack(list, ItemInit(QuickAllocatedString("teststring3"),
                                       &ItemValueStringOperations));

    // LinkedListPrint((LinkedList *)list);
    // head shouldn't change
    TestCaseVerify(true, "ensure the head didn't change when we add to back",
                   strcmp(list->head->item->value, "teststring2") == 0);
    TestCaseVerify(true, "ensure head->next was unchanged",
                   strcmp(list->head->next->item->value, "teststring1") == 0);
    TestCaseVerify(
        true, "ensure the item added to the end has the correct value",
        strcmp(list->head->next->next->item->value, "teststring3") == 0);

    LinkedListDeleteBack(list);
    TestCaseVerify(
        true, "after deleting the last item, ensure the previous->next is NULL",
        list->head->next->next == NULL);

    LinkedListFree(list);
}

static void toStringTest()
{
    LinkedList *list = LinkedListInit();
    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring1"),
                                        &ItemValueStringOperations));
    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring2"),
                                        &ItemValueStringOperations));
    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring3"),
                                        &ItemValueStringOperations));

    char *as_string = LinkedListToString((LinkedList *)list);
    TestCaseVerify(true, "ensure LinkedListToString doesn't return NULL",
                   as_string != NULL);

    // printf("%s\n", as_string);

    LinkedListFree(list);
    free(as_string);
}

extern void TestLinkedList()
{
    basicTest();
    toStringTest();
}
