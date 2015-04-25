#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daRemoveRange(NULL, NULL, 0, 0) != -1, "daRemoveRange(NULL, NULL, 0, 0) != -1");
    sput_fail_if(daRemoveRange(&da,  NULL, 0, 0) != -1, "daRemoveRange(&da,  NULL, 0, 0) != -1");
    sput_fail_if(daRemoveRange(NULL, &err, 0, 0) != -1, "daRemoveRange(NULL, &err, 0, 0) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testRemoveRange(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;
    const char *src;
    const char *expect;

    desc.elements = 5;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "12345";
    memcpy(da->firstAddr, src, 5);
    da->used = 5;

    sput_fail_if(daRemoveRange(da, &err, 0, 1) != 0, "daRemoveRange should delete existing elements (0,1)");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    expect = "345";
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daRemoveRange should remove elements (0,1)");
    sput_fail_if(da->used != 3, "daRemoveRange should update used counter (0,1)");


    src = "12345";
    memcpy(da->firstAddr, src, 5);
    da->used = 5;

    sput_fail_if(daRemoveRange(da, &err, 3, 4) != 0, "daRemoveRange should delete existing elements (3,4)");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    expect = "123";
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daRemoveRange should remove elements (3,4)");
    sput_fail_if(da->used != 3, "daRemoveRange should update used counter (3,4)");


    src = "12345";
    memcpy(da->firstAddr, src, 5);
    da->used = 5;

    sput_fail_if(daRemoveRange(da, &err, 1, 4) != 0, "daRemoveRange should delete existing elements (1,4)");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    expect = "1";
    sput_fail_if(memcmp(da->firstAddr, expect, 1) != 0, "daRemoveRange should remove elements (1,4)");
    sput_fail_if(da->used != 1, "daRemoveRange should update used counter (1,4)");


    src = "12345";
    memcpy(da->firstAddr, src, 5);
    da->used = 5;

    sput_fail_if(daRemoveRange(da, &err, 0, 4) != 0, "daRemoveRange should delete existing elements (0,4)");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    sput_fail_if(da->used != 0, "daRemoveRange should update used counter (0,4)");
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.used = 3;

    sput_fail_if(daRemoveRange(&da, &err, 1, 0) != -1, "daRemoveRange should fail if from > to");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daRemoveRange(&da, &err, 0, da.used) != -1, "daRemoveRange should fail if to is out of bounds");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daRemoveRange(&da, &err, da.used, da.used) != -1, "daRemoveRange should fail if from is out of bounds");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daRemoveRange(&da, &err, 0, 0) != -1, "daRemoveRange should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daRemoveRange should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daRemoveRange should remove existing elements");
    sput_run_test(testRemoveRange);

    sput_enter_suite("daRemoveRange should fail if any index is out of bounds");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daRemoveRange should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}