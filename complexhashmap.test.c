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

    Item *key = ItemInitV2(strdup("key"), &ItemValueStringOperations,
                           &SL_COLLECTIONS_TYPE_STRING);
    ComplexHashMapInsert(map, key,
                         ItemInitV2(strdup("value"), &ItemValueStringOperations,
                                    &SL_COLLECTIONS_TYPE_STRING));
    TestCaseVerify(true, "check if inserted item is there",
                   strcmp(ComplexHashMapGet(map, key)->value, "value") == 0);

    // this is getting crazy

    List *list = ListInitDefault();
    ListAddFirst(list,
                 ItemInitV2(strdup("listvalue1"), &ItemValueStringOperations,
                            &SL_COLLECTIONS_TYPE_STRING));
    ListAddFirst(list,
                 ItemInitV2(strdup("listvalue2"), &ItemValueStringOperations,
                            &SL_COLLECTIONS_TYPE_STRING));

    Item *list_key =
        ItemInitV2(list, &ItemValueListOperations, &SL_COLLECTIONS_TYPE_LIST);
    ComplexHashMapInsert(map, list_key,
                         ItemInitV2(strdup("value_where_list_is_key"),
                                    &ItemValueStringOperations,
                                    &SL_COLLECTIONS_TYPE_STRING));

    TestCaseVerify(true, "check if inserted item is there with list key",
                   strcmp(ComplexHashMapGet(map, list_key)->value,
                          "value_where_list_is_key") == 0);

    // complexhashmap as key now
    ComplexHashMap *map_will_be_key = ComplexHashMapInitDefault();
    ComplexHashMapInsert(
        map_will_be_key,
        ItemInitV2(strdup("key_here"), &ItemValueStringOperations,
                   &SL_COLLECTIONS_TYPE_STRING),
        ItemInitV2(strdup("value_key"), &ItemValueStringOperations,
                   &SL_COLLECTIONS_TYPE_STRING));

    Item *map_key =
        ItemInitV2(map_will_be_key, &ItemValueComplexHashMapOperations,
                   &SL_COLLECTIONS_TYPE_COMPLEX_HASHMAP);

    ComplexHashMapInsert(map, map_key,
                         ItemInitV2(strdup("value_where_key_is_cmap"),
                                    &ItemValueStringOperations,
                                    &SL_COLLECTIONS_TYPE_STRING));

    ComplexHashMapPrint(map);
    printf("\n");
    fflush(stdout);
    ComplexHashMapFree(map);
}
