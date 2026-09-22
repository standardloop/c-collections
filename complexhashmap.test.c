#include <standardloop/testing.h>
#include <standardloop/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./collections.h"

extern void TestComplexHashMap()
{
    ComplexHashMap *map = ComplexHashMapInitDefault();
    TestCaseVerify(true, "", map != NULL);

    Item *key = ItemInitV2(strdup("key"), &SL_COLLECTIONS_TYPE_STRING,
                           &ItemValueStringOperations);
    ComplexHashMapInsert(map, key,
                         ItemInitV2(strdup("value"),
                                    &SL_COLLECTIONS_TYPE_STRING,
                                    &ItemValueStringOperations));
    TestCaseVerify(true, "check if inserted item is there",
                   strcmp(ComplexHashMapGet(map, key)->value, "value") == 0);

    // this is getting crazy

    List *list = ListInitDefault();
    ListAddFirst(list,
                 ItemInitV2(strdup("listvalue1"), &SL_COLLECTIONS_TYPE_STRING,
                            &ItemValueStringOperations));
    ListAddFirst(list,
                 ItemInitV2(strdup("listvalue2"), &SL_COLLECTIONS_TYPE_STRING,
                            &ItemValueStringOperations));

    Item *list_key =
        ItemInitV2(list, &SL_COLLECTIONS_TYPE_LIST, &ItemValueListOperations);
    ComplexHashMapInsert(map, list_key,
                         ItemInitV2(strdup("value_where_list_is_key"),
                                    &SL_COLLECTIONS_TYPE_STRING,
                                    &ItemValueStringOperations));

    TestCaseVerify(true, "check if inserted item is there with list key",
                   strcmp(ComplexHashMapGet(map, list_key)->value,
                          "value_where_list_is_key") == 0);

    // complexhashmap as key now
    ComplexHashMap *map_will_be_key = ComplexHashMapInitDefault();
    ComplexHashMapInsert(
        map_will_be_key,
        ItemInitV2(strdup("key_here"), &SL_COLLECTIONS_TYPE_STRING,
                   &ItemValueStringOperations),
        ItemInitV2(strdup("value_key"), &SL_COLLECTIONS_TYPE_STRING,
                   &ItemValueStringOperations));

    Item *map_key =
        ItemInitV2(map_will_be_key, &SL_COLLECTIONS_TYPE_COMPLEX_HASHMAP,
                   &ItemValueComplexHashMapOperations);

    ComplexHashMapInsert(map, map_key,
                         ItemInitV2(strdup("value_where_key_is_cmap"),
                                    &SL_COLLECTIONS_TYPE_STRING,
                                    &ItemValueStringOperations));

    ComplexHashMapPrint(map);
    printf("\n");
    fflush(stdout);
    ComplexHashMapFree(map);
}
