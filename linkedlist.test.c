#include "./collections.h"
#include <assert.h>
#include <standardloop/util.h>

extern void TestLinkedList()
{

    LinkedList *list = LinkedListInit();
    assert(list != NULL);
    assert(IsLinkedListEmpty(list));

    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring1"),
                                        &ItemValueStringOperations));

    assert(list->size == 1);

    assert(strcmp(list->head->item->value, "teststring1") == 0);

    LinkedListAddToFront(list, ItemInit(QuickAllocatedString("teststring2"),
                                        &ItemValueStringOperations));

    assert(strcmp(list->head->item->value, "teststring2") == 0);

    LinkedListAddToBack(list, ItemInit(QuickAllocatedString("teststring3"),
                                       &ItemValueStringOperations));

    // head shouldn't change
    assert(strcmp(list->head->item->value, "teststring2") == 0);
    assert(strcmp(list->head->next->item->value, "teststring1") == 0);
    assert(strcmp(list->head->next->next->item->value, "teststring3") == 0);

    LinkedListDeleteBack(list);
    assert(list->head->next->next == NULL);

    LinkedListFree(list);
}
