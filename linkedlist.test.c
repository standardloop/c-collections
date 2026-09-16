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

    LinkedListFree(list);
}
