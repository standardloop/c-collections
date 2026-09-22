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
    ComplexHashMapFree(map);
}
