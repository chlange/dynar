#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daRemove(NULL, NULL, 0) != -1, "daRemove(NULL, NULL, 0) != -1");
    sput_fail_if(daRemove(&da,  NULL, 0) != -1, "daRemove(&da,  NULL, 0) != -1");
    sput_fail_if(daRemove(NULL, &err, 0) != -1, "daRemove(NULL, &err, 0) != -1");
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
    desc.maxBytes = 10;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    sput_fail_if(daRemove(da, &err, 0) != 0, "daRemove should remove the first element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 2, "daRemove should decrease the used counter");
    expect = "23";
    sput_fail_if(memcmp(da->firstAddr, expect, 2), "daRemove should shift all other elements to the left");


    sput_fail_if(daRemove(da, &err, 1) != 0, "daRemove should remove the second (endmost) element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 1, "daRemove should decrease the used counter");
    expect = "2";
    sput_fail_if(memcmp(da->firstAddr, expect, 1), "daRemove should remain the last remaining (not touched) element at the position");


    sput_fail_if(daRemove(da, &err, 0) != 0, "daRemove should remove the last element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 0, "daRemove should decrease the used counter");



    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    sput_fail_if(daRemove(da, &err, 2) != 0, "daRemove should remove the third (endmost) element and succeed");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 2, "daRemove should decrease the used counter");
    expect = "12";
    sput_fail_if(memcmp(da->firstAddr, expect, 1), "daRemove should remain the last remaining (not touched) elements at the positions");

    daDestroy(da, &err);
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daRemove(&da, &err, 0) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemove(&da, &err, 1) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemove(&da, &err, 2) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");

    da.used = 1;
    sput_fail_if(daRemove(&da, &err, 1) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemove(&da, &err, 2) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");

    da.used = 10;
    sput_fail_if(daRemove(&da, &err, 10) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daRemove(&da, &err, 11) != -1, "daRemove should return -1 if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daRemove(&da, &err, 0) != -1, "daRemove should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daRemove should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daRemove should remove an existing element");
    sput_run_test(testRemove);

    sput_enter_suite("daRemove should fail if pos is out of bounds");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daRemove should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}