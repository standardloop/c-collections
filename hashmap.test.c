#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

static u_int32_t collisionHashFunction(char *key, u_int32_t capacity)
{
    if (key == NULL || capacity == 0)
    {
        return 0;
    }
    return 0;
}

static void testCollisions()
{
    HashMap *collision_map = HashMapInit(10, 2, collisionHashFunction, false);
    TestCaseVerify(true, "ensure collision_map is not NULL",
                   collision_map != NULL);

    HashMapInsert(collision_map,
                  HashMapItemInit(QuickAllocatedString("key1"),
                                  ItemInit(QuickAllocatedString("value1"),
                                           &ItemValueStringOperations)));
    TestCaseVerify(
        true, "get works",
        strcmp(HashMapGet(collision_map, "key1")->item->value, "value1") == 0);

    TestCaseVerify(true, "no collision since one insert only",
                   HashMapGet(collision_map, "key1")->next == NULL);

    HashMapInsert(collision_map,
                  HashMapItemInit(QuickAllocatedString("key2"),
                                  ItemInit(QuickAllocatedString("value2"),
                                           &ItemValueStringOperations)));

    TestCaseVerify(
        true, "get works on 2nd insert",
        strcmp(HashMapGet(collision_map, "key2")->item->value, "value2") == 0);

    TestCaseVerify(true, "there should be a collision",
                   HashMapGet(collision_map, "key1")->next != NULL);

    HashMapFree(collision_map);
}

static void testHashMapItemDuplicate()
{
    char *test_key_1 = QuickAllocatedString("test_key_1");
    TestCaseVerify(true, "", test_key_1 != NULL);

    int *test_int_1 = malloc(sizeof(int));
    TestCaseVerify(true, "", test_int_1 != NULL);
    *test_int_1 = 100;
    TestCaseVerify(true, "", *test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
    TestCaseVerify(true, "", test_item_1 != NULL);
    TestCaseVerify(true, "", *(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    TestCaseVerify(true, "", test_hashmap_item_1 != NULL);
    TestCaseVerify(true, "",
                   strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    TestCaseVerify(true, "", *(int *)test_hashmap_item_1->item->value == 100);

    // ---

    HashMapItem *dupe = HashMapItemDuplicate(test_hashmap_item_1);
    TestCaseVerify(true, "", dupe != NULL);

    TestCaseVerify(true, "",
                   test_hashmap_item_1->key !=
                       dupe->key); // check memory address
    TestCaseVerify(true, "", strcmp(test_hashmap_item_1->key, dupe->key) == 0);

    TestCaseVerify(true, "",
                   test_hashmap_item_1->item->value !=
                       dupe->item->value); // check memory address
    TestCaseVerify(true, "",
                   *(int *)test_hashmap_item_1->item->value ==
                       *(int *)dupe->item->value);

    HashMapItemFree(test_hashmap_item_1, true);
    HashMapItemFree(dupe, true);
}

static void testHashMapItem()
{
    char *test_key_1 = QuickAllocatedString("test_key_1");
    TestCaseVerify(true, "", test_key_1 != NULL);

    int *test_int_1 = malloc(sizeof(int));
    TestCaseVerify(true, "", test_int_1 != NULL);
    *test_int_1 = 100;
    TestCaseVerify(true, "", *test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
    TestCaseVerify(true, "", test_item_1 != NULL);
    TestCaseVerify(true, "", *(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    TestCaseVerify(true, "", test_hashmap_item_1 != NULL);
    TestCaseVerify(true, "",
                   strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    TestCaseVerify(true, "", *(int *)test_hashmap_item_1->item->value == 100);

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
    TestCaseVerify(true, "", map != NULL);
    HashMapInsert(map, HashMapItemInit(QuickAllocatedString("key"),
                                       ItemInit(QuickAllocatedString("value"),
                                                &ItemValueStringOperations)));

    // TestCaseVerify(true, "",strcmp(HashMapGet(map, "key")->item->value,
    // "value") == 0);

    HashMap *dupe = HashMapDuplicate(map);
    TestCaseVerify(true, "", dupe != NULL);

    TestCaseVerify(true, "", dupe != map);

    // HashMapPrint(dupe);
    // printf("\n-------------------\n");
    // HashMapPrint(map);

    TestCaseVerify(true, "",
                   strcmp(HashMapGet(dupe, "key")->item->value, "value") == 0);

    TestCaseVerify(true, "",
                   strcmp(HashMapGet(map, "key")->item->value,
                          HashMapGet(dupe, "key")->item->value) == 0);

    TestCaseVerify(true, "", map->force_lowercase == dupe->force_lowercase);
    TestCaseVerify(true, "", map->capacity == dupe->capacity);
    TestCaseVerify(true, "", map->resize_multiple == dupe->resize_multiple);
    TestCaseVerify(true, "", map->size == dupe->size);
    TestCaseVerify(true, "", map->hashFunction == dupe->hashFunction);

    HashMapFree(map);
    HashMapFree(dupe);
}

extern void TestHashMap()
{
    testCollisions();
    return;

    testHashMapToString();

    testHashMapItem();

    testHashMapDuplicate();

    // setup first key
    char *test_key_1 = QuickAllocatedString("test_key_1");
    TestCaseVerify(true, "", test_key_1 != NULL);

    // setup item
    int *test_int_1 = malloc(sizeof(int));
    TestCaseVerify(true, "", test_int_1 != NULL);
    *test_int_1 = 100;
    TestCaseVerify(true, "", *test_int_1 == 100);
    Item *test_item_1 = ItemInit(test_int_1, &ItemValueIntOperations);
    TestCaseVerify(true, "", test_item_1 != NULL);
    TestCaseVerify(true, "", *(int *)test_item_1->value == 100);

    HashMapItem *test_hashmap_item_1 = HashMapItemInit(test_key_1, test_item_1);
    TestCaseVerify(true, "", test_hashmap_item_1 != NULL);
    TestCaseVerify(true, "",
                   strcmp(test_hashmap_item_1->key, "test_key_1") == 0);
    TestCaseVerify(true, "", *(int *)test_hashmap_item_1->item->value == 100);

    // setup hashmap
    HashMap *test_hashmap_1 = HashMapInit(1, 2, NULL, false);
    TestCaseVerify(true, "", test_hashmap_1 != NULL);

    // test insertion and retrieval
    HashMapInsert(test_hashmap_1, test_hashmap_item_1);
    TestCaseVerify(true, "", test_hashmap_1->size == 1);
    HashMapItem *retrieved = HashMapGet(test_hashmap_1, "test_key_1");
    TestCaseVerify(true, "", retrieved != NULL);
    TestCaseVerify(true, "", *(int *)retrieved->item->value == 100);

    // setup 2
    //// key 2
    char *test_key_2 = QuickAllocatedString("test_key_2");
    TestCaseVerify(true, "", test_key_2 != NULL);

    //// int 2
    int *test_int_2 = malloc(sizeof(int));
    TestCaseVerify(true, "", test_int_2 != NULL);
    *test_int_2 = 200;
    TestCaseVerify(true, "", *test_int_2 == 200);

    //// item 2
    Item *test_item_2 = ItemInit(test_int_2, &ItemValueIntOperations);
    TestCaseVerify(true, "", test_item_2 != NULL);
    TestCaseVerify(true, "", *(int *)test_item_2->value == 200);

    //// hashmap item 2
    HashMapItem *test_hashmap_item_2 = HashMapItemInit(test_key_2, test_item_2);
    TestCaseVerify(true, "", test_hashmap_item_2 != NULL);
    TestCaseVerify(true, "",
                   strcmp(test_hashmap_item_2->key, "test_key_2") == 0);
    TestCaseVerify(true, "", *(int *)test_hashmap_item_2->item->value == 200);

    //// insert and retrieval
    HashMapInsert(test_hashmap_1, test_hashmap_item_2);
    HashMapItem *retrieved_2 = HashMapGet(test_hashmap_1, "test_key_2");
    TestCaseVerify(true, "", retrieved_2 != NULL);
    TestCaseVerify(true, "", *(int *)retrieved_2->item->value == 200);

    HashMapFree(test_hashmap_1);
}
