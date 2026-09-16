#include <assert.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static void testHashMapItemDuplicate()
{
    char *test_key_1 = QuickAllocatedString("test_key_1");
    assert(test_key_1 != NULL);

    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    assert(*test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
    assert(test_item_1 != NULL);
    assert(*(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    assert(test_hashmap_item_1 != NULL);
    assert(strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    assert(*(int *)test_hashmap_item_1->item->value == 100);

    // ---

    HashMapItem *dupe = HashMapItemDuplicate(test_hashmap_item_1);
    assert(dupe != NULL);

    assert(test_hashmap_item_1->key != dupe->key); // check memory address
    assert(strcmp(test_hashmap_item_1->key, dupe->key) == 0);

    assert(test_hashmap_item_1->item->value !=
           dupe->item->value); // check memory address
    assert(*(int *)test_hashmap_item_1->item->value ==
           *(int *)dupe->item->value);

    HashMapItemFree(test_hashmap_item_1, true);
    HashMapItemFree(dupe, true);
}

static void testHashMapItem()
{
    char *test_key_1 = QuickAllocatedString("test_key_1");
    assert(test_key_1 != NULL);

    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    assert(*test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
    assert(test_item_1 != NULL);
    assert(*(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    assert(test_hashmap_item_1 != NULL);
    assert(strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    assert(*(int *)test_hashmap_item_1->item->value == 100);

    HashMapItemFree(test_hashmap_item_1, true);

    testHashMapItemDuplicate();
}

static void testHashMapToString()
{
    HashMap *map = HashMapInitDefault();
    HashMapInsert(map, HashMapItemInit(QuickAllocatedString("key"),
                                       ItemInit(QuickAllocatedString("value"),
                                                &ItemValueStringOperations)));

    char *as_string = HashMapToString(map);
    // printf("%s\n", as_string);
    free(as_string);
    HashMapFree(map);
}

static void testHashMapDuplicate()
{
    HashMap *map = HashMapInitDefault();
    assert(map != NULL);
    HashMapInsert(map, HashMapItemInit(QuickAllocatedString("key"),
                                       ItemInit(QuickAllocatedString("value"),
                                                &ItemValueStringOperations)));

    // assert(strcmp(HashMapGet(map, "key")->item->value, "value") == 0);

    HashMap *dupe = HashMapDuplicate(map);
    assert(dupe != NULL);

    assert(dupe != map);

    // HashMapPrint(dupe);
    // printf("\n-------------------\n");
    // HashMapPrint(map);

    assert(strcmp(HashMapGet(dupe, "key")->item->value, "value") == 0);

    assert(strcmp(HashMapGet(map, "key")->item->value,
                  HashMapGet(dupe, "key")->item->value) == 0);

    assert(map->force_lowercase == dupe->force_lowercase);
    assert(map->capacity == dupe->capacity);
    assert(map->resize_multiple == dupe->resize_multiple);
    assert(map->size == dupe->size);
    assert(map->hashFunction == dupe->hashFunction);

    HashMapFree(map);
    HashMapFree(dupe);
}

extern void TestHashMap()
{
    testHashMapToString();

    testHashMapItem();

    testHashMapDuplicate();

    // setup first key
    char *test_key_1 = QuickAllocatedString("test_key_1");
    assert(test_key_1 != NULL);

    // setup item
    int *test_int_1 = malloc(sizeof(int));
    assert(test_int_1 != NULL);
    *test_int_1 = 100;
    assert(*test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
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
    Item *test_item_2 = ItemInit(test_int_2, &ItemValueIntOperations);
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
                                           &ItemValueStringOperations)));
    HashMapInsert(collision_test,
                  HashMapItemInit(QuickAllocatedString("two"),
                                  ItemInit(QuickAllocatedString("two-value"),
                                           &ItemValueStringOperations)));

    assert(collision_test->collision_count == 1);
    HashMapItem *retrieved_collision_1 = HashMapGet(collision_test, "one");
    assert(retrieved_collision_1 != NULL);
    assert(strcmp(retrieved_collision_1->item->value, "one-value") == 0);

    HashMapItem *retrieved_collision_2 = HashMapGet(collision_test, "two");
    assert(retrieved_collision_2 != NULL);
    assert(strcmp(retrieved_collision_2->item->value, "two-value") == 0);

    HashMapFree(collision_test);
}
