#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daRemoveDirty(NULL, NULL, 0) != -1, "daRemoveDirty(NULL, NULL, 0) != -1");
    sput_fail_if(daRemoveDirty(&da,  NULL, 0) != -1, "daRemoveDirty(&da,  NULL, 0) != -1");
    sput_fail_if(daRemoveDirty(NULL, &err, 0) != -1, "daRemoveDirty(NULL, &err, 0) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testRemove(void)
{
    int err;
    const char *src;
    const char *expect;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->freeAddr = (char *)da->firstAddr + 3;
    da->used = 3;

    sput_fail_if(daRemoveDirty(da, &err, 0) != 0, "daRemoveDirty should remove the first element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 2, "daRemoveDirty should decrease the used counter");
    expect = "32";
    sput_fail_if(memcmp(da->firstAddr, expect, 2), "daRemoveDirty should shift all other elements to the left");
    sput_fail_if(da->freeAddr != (char *)da->firstAddr + 2, "daRemoveDirty should update the freeAddr pointer");


    sput_fail_if(daRemoveDirty(da, &err, 1) != 0, "daRemoveDirty should remove the second (endmost) element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 1, "daRemoveDirty should decrease the used counter");
    expect = "3";
    sput_fail_if(memcmp(da->firstAddr, expect, 1), "daRemoveDirty should remain the last remaining (not touched) element at the position");
    sput_fail_if(da->freeAddr != (char *)da->firstAddr + 1, "daRemoveDirty should update the freeAddr pointer");


    sput_fail_if(daRemoveDirty(da, &err, 0) != 0, "daRemoveDirty should remove the last element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 0, "daRemoveDirty should decrease the used counter");
    sput_fail_if(da->freeAddr != da->firstAddr, "daRemoveDirty should update the freeAddr pointer");



    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->freeAddr = (char *)da->firstAddr + 3;
    da->used = 3;

    sput_fail_if(daRemoveDirty(da, &err, 2) != 0, "daRemoveDirty should remove the third (endmost) element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 2, "daRemoveDirty should decrease the used counter");
    expect = "12";
    sput_fail_if(memcmp(da->firstAddr, expect, 1), "daRemoveDirty should remain the last remaining (not touched) elements at the positions");
    sput_fail_if(da->freeAddr != (char *)da->firstAddr + 2, "daRemoveDirty should update the freeAddr pointer");

    daDestroy(da, &err);
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daRemoveDirty(&da, &err, 0) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemoveDirty(&da, &err, 1) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemoveDirty(&da, &err, 2) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");

    da.used = 1;
    sput_fail_if(daRemoveDirty(&da, &err, 1) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemoveDirty(&da, &err, 2) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");

    da.used = 10;
    sput_fail_if(daRemoveDirty(&da, &err, 10) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemoveDirty(&da, &err, 11) != -1, "daRemoveDirty should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daRemoveDirty(&da, &err, 0) != -1, "daRemoveDirty should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daRemoveDirty should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daRemoveDirty should remove an existing element");
    sput_run_test(testRemove);

    sput_enter_suite("daRemoveDirty should fail if pos is out of bounds");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daRemoveDirty should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}