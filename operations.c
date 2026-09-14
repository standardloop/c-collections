#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#include "./collections.h"

ItemValueOperations ItemValueStringOperations = {
    .toStringFunction = StringToString,
    .freeFunction = free,
    .printFunction = ItemPrintString};

ItemValueOperations ItemValueIntOperations = {.toStringFunction = IntToString,
                                              .freeFunction = free,
                                              .printFunction = ItemPrintInt};

ItemValueOperations ItemValueListOperations = {.toStringFunction = ListToString,
                                               .freeFunction = ListFree,
                                               .printFunction = ListPrint};

ItemValueOperations ItemValueHashMapOperations = {
    .toStringFunction = HashMapToString,
    .freeFunction = HashMapFree,
    .printFunction = HashMapPrint};
