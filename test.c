#include <assert.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

void printInt(void *value)
{
    if (value != NULL)
    {
        printf("%d", *(int *)value);
    }
}

static void testHashMapItem()
{
    char *test_key_1 = QuickAllocatedString("test_key_1");
    assert(test_key_1 != NULL);

    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    assert(*test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, free, printInt, NULL);
    assert(test_item_1 != NULL);
    assert(*(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    assert(test_hashmap_item_1 != NULL);
    assert(strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    assert(*(int *)test_hashmap_item_1->item->value == 100);

    HashMapItemFree(test_hashmap_item_1, true);
}

static void testHashMap()
{
    // setup first key
    char *test_key_1 = QuickAllocatedString("test_key_1");
    assert(test_key_1 != NULL);

    // setup item
    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    assert(*test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, free, printInt, NULL);
    assert(test_item_1 != NULL);
    assert(*(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    assert(test_hashmap_item_1 != NULL);
    assert(strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    assert(*(int *)test_hashmap_item_1->item->value == 100);

    // setup hashmap
    HashMap *test_hashmap_1 = HashMapInit(1, 2, NULL, false);
    assert(test_hashmap_1 != NULL);

    // test insertion and retrieval
    HashMapInsert(test_hashmap_1, test_hashmap_item_1);
    assert(test_hashmap_1->size == 1);
    HashMapItem *retrieved = HashMapGet(test_hashmap_1, "test_key_1");
    assert(retrieved != NULL);
    assert(*(int *)retrieved->item->value == 100);

    // setup 2
    //// key 2
    char *test_key_2 = QuickAllocatedString("test_key_2");
    assert(test_key_2 != NULL);

    //// int 2
    int *test_int_2 = malloc(sizeof(int));
    assert(test_int_2 != NULL);
    *test_int_2 = 200;
    assert(*test_int_2 == 200);

    //// item 2
    Item *test_item_2 = ItemInit(test_int_2, free, printInt, NULL);
    assert(test_item_2 != NULL);
    assert(*(int *)test_item_2->value == 200);

    //// hashmap item 2
    HashMapItem *test_hashmap_item_2 = HashMapItemInit(test_key_2, test_item_2);
    assert(test_hashmap_item_2 != NULL);
    assert(strcmp(test_hashmap_item_2->key, "test_key_2") == 0);
    assert(*(int *)test_hashmap_item_2->item->value == 200);

    //// insert and retrieval
    HashMapInsert(test_hashmap_1, test_hashmap_item_2);
    HashMapItem *retrieved_2 = HashMapGet(test_hashmap_1, "test_key_2");
    assert(retrieved_2 != NULL);
    assert(*(int *)retrieved_2->item->value == 200);

    HashMapFree(test_hashmap_1);

    // test collisions
    HashMap *collision_test = HashMapInit(2, 2, NULL, false);
    assert(collision_test != NULL);

    // defaultHashFunction("one", 2) and defaultHashFunction("two", 2) both
    // return index 0
    HashMapInsert(collision_test,
                  HashMapItemInit(QuickAllocatedString("one"),
                                  ItemInit(QuickAllocatedString("one-value"),
                                           free, ItemPrintString, NULL)));
    HashMapInsert(collision_test,
                  HashMapItemInit(QuickAllocatedString("two"),
                                  ItemInit(QuickAllocatedString("two-value"),
                                           free, ItemPrintString, NULL)));

    assert(collision_test->collision_count == 1);
    HashMapItem *retrieved_collision_1 = HashMapGet(collision_test, "one");
    assert(retrieved_collision_1 != NULL);
    assert(strcmp(retrieved_collision_1->item->value, "one-value") == 0);

    HashMapItem *retrieved_collision_2 = HashMapGet(collision_test, "two");
    assert(retrieved_collision_2 != NULL);
    assert(strcmp(retrieved_collision_2->item->value, "two-value") == 0);

    HashMapFree(collision_test);
}

int main()
{
    // TestItem();
    TestList();
    sleep(1);
    return EXIT_SUCCESS;
    testHashMapItem();
    testHashMap();
    sleep(1);

    return EXIT_SUCCESS;
}
