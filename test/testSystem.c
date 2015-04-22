#include "dynar.h"
#include "sput.h"

static void testSystem(void)
{
    int i;
    int err;
    char buf[150];
    void *retPtr;
    size_t size;
    size_t index;
    DaDesc desc;
    DaStruct *da;
    DaStruct *originalDa;

    desc.elements = 1;
    desc.bytesPerElement = 50;

    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array");

    for (i = 0; i < 10000; i++)
    {
        sprintf(buf, "%050d", i);
        sput_fail_if(daAppend(da, &err, buf) == NULL, "Unable to append an element to the dynamic array");
    }

    sput_fail_if(daIsEmpty(da, &err) != 0, "daIsEmpty should return 0 if the array isn't empty");

    retPtr = daGet(da, &err, 0);
    sput_fail_if(daGetFirst(da, &err) != retPtr, "daGetFirst should return the same as daGet with index 0");

    sput_fail_if(daSize(da, &err, &size) != 0, "daSize should return 0");
    retPtr = daGet(da, &err, size - 1);
    sput_fail_if(daGetLast(da, &err) != retPtr, "daGetLast should return the same as daGet with the last index");

    for (i = 0; i < 10000; i++)
    {
        sprintf(buf, "%050d", i);
        sput_fail_if(memcmp(daGet(da, &err, i), buf, desc.bytesPerElement) != 0, "daGet should return the same as the daAppend input");

        sput_fail_if(daIndexOf(da, &err, buf, &index) != 1, "daIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daIndexOf should return the correct index");

        retPtr = daGet(da, &err, i);
        sput_fail_if(daSet(da, &err, buf, i) != retPtr, "daSet should return the same pointer as daGet for same index");

        sput_fail_if(daLastIndexOf(da, &err, buf, &index) != 1, "daLastIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daLastIndexOf should return the correct index");

        sput_fail_if(daContains(da, &err, buf) != 1, "daContains should return 1 because the element exists");

        sprintf(buf, "%050d", 999999);
        sput_fail_if(daIndexOf(da, &err, buf, &index) != 0, "daIndexOf should return 0 because the element doesn't exists");

        sprintf(buf, "%050d", 999999);
        sput_fail_if(daLastIndexOf(da, &err, buf, &index) != 0, "daLastIndexOf should return 0 because the element doesn't exists");
    }
    for (i = 0; i < 10000; i++)
    {
        sput_fail_if(daRemove(da, &err, da->used - 1) != 0, "daRemove should remove existing element");
    }
    sput_fail_if(daRemove(da, &err, da->used - 1) != -1, "daRemove should return -1 if no element exists");
    sput_fail_if(daClear(da, &err, DA_SOFT) != 0, "daClear should return 0");
    sput_fail_if(da->used != 0, "daClear should set da->used to 0");

    sput_fail_if(daIsEmpty(da, &err) != 1, "daIsEmpty should return 1 if the array is empty");


    originalDa = da;
    da = daClone(da, &err);
    sput_fail_if(da == NULL, "daClone should clone the array");
    sput_fail_if(da == originalDa, "daClone should create a new array");
    sput_fail_if(daDestroy(originalDa, &err) != 0, "daDestroy should destroy the original array");

    i = 10000;
    while (i--)
    {
        sprintf(buf, "%050d", i);
        sput_fail_if(daPrepend(da, &err, buf) == NULL, "Unable to prepend an element to the dynamic array");
    }

    sput_fail_if(daIsEmpty(da, &err) != 0, "daIsEmpty should return 0 if the array isn't empty");

    retPtr = daGet(da, &err, 0);
    sput_fail_if(daGetFirst(da, &err) != retPtr, "daGetFirst should return the same as daGet with index 0");

    sput_fail_if(daSize(da, &err, &size) != 0, "daSize should return 0");
    retPtr = daGet(da, &err, size - 1);
    sput_fail_if(daGetLast(da, &err) != retPtr, "daGetLast should return the same as daGet with the last index");

    for (i = 0; i < 10000; i++)
    {
        sprintf(buf, "%050d", i);
        sput_fail_if(memcmp(daGet(da, &err, i), buf, desc.bytesPerElement) != 0, "daGet should return the same as the daPrepend input");

        sput_fail_if(daIndexOf(da, &err, buf, &index) != 1, "daIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daIndexOf should return the correct index");

        retPtr = daGet(da, &err, i);
        sput_fail_if(daSet(da, &err, buf, i) != retPtr, "daSet should return the same pointer as daGet for same index");

        sput_fail_if(daLastIndexOf(da, &err, buf, &index) != 1, "daLastIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daLastIndexOf should return the correct index");

        sput_fail_if(daContains(da, &err, buf) != 1, "daContains should return 1 because the element exists");

        sprintf(buf, "%050d", 999999);
        sput_fail_if(daIndexOf(da, &err, buf, &index) != 0, "daIndexOf should return 0 because the element doesn't exists");

        sprintf(buf, "%050d", 999999);
        sput_fail_if(daLastIndexOf(da, &err, buf, &index) != 0, "daLastIndexOf should return 0 because the element doesn't exists");
    }

    i = 10000;
    while (i--)
    {
        sprintf(buf, "%050d", i);
        sput_fail_if(daIndexOf(da, &err, buf, &index) != 1, "daIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daIndexOf should return the correct index");

        sput_fail_if(daLastIndexOf(da, &err, buf, &index) != 1, "daLastIndexOf should return 1 because the element exists");
        sput_fail_if(index != i, "daLastIndexOf should return the correct index");
    }

    for (i = 0; i < 10000; i++)
    {
        sput_fail_if(daRemove(da, &err, da->used - 1) != 0, "daRemove should remove an existing element");
    }
    sput_fail_if(daRemove(da, &err, da->used - 1) != -1, "daRemove should return -1 if no element exists");
    sput_fail_if(daClear(da, &err, DA_HARD) != 0, "daClear should return 0");
    sput_fail_if(da->used != 0, "daClear should set da->used to 0");

    sput_fail_if(daIsEmpty(da, &err) != 1, "daIsEmpty should return 1 if the array is empty");

    sput_fail_if(daDestroy(da, &err) != 0, "daDestroy should destroy the array");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("System test");
    sput_run_test(testSystem);

    sput_finish_testing();

    return sput_get_return_value();
}