#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daClear(NULL, NULL, 0) != -1, "daClear(NULL, NULL, 0) != -1");
    sput_fail_if(daClear(&da,  NULL, 0) != -1, "daClear(&da,  NULL, 0) != -1");
    sput_fail_if(daClear(NULL, &err, 0) != -1, "daClear(NULL, &err, 0) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
{
    int err;
    const char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 10;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    da->freeAddr = (void *)1;
    sput_fail_if(daClear(da, &err, DA_FAST) != 0, "daClear should succeed if we clear fast");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 0, "daClear should reset the used counter");
    sput_fail_if(da->freeAddr != da->firstAddr, "daClear should reset the freeAddr pointer");



    src = "5555555555";
    memcpy(da->firstAddr, src, 10);
    da->used = 10;

    da->freeAddr = (void *)1;
    sput_fail_if(daClear(da, &err, DA_SECURE) != 0, "daClear should succeed if we clear fast");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->used != 0, "daClear should reset the used counter");
    sput_fail_if(da->freeAddr != da->firstAddr, "daClear should reset the freeAddr pointer");
    src = "0000000000";
    sput_fail_if(memcmp(da->firstAddr, src, 10) != 0, "daClear in secure mode doesn't erase the array content");
}

static void testUnknownMode(void)
{
    int err;
    DaStruct da;
    da.magic = DA_MAGIC;

    sput_fail_if(daClear(&da, &err, 123456) != -1, "daClear should fail if we pass an unknown mode");
    sput_fail_if(err != (DA_PARAM_ERR | DA_UNKNOWN_MODE), "err != (DA_PARAM_ERR | DA_UNKNOWN_MODE)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daClear(&da, &err, 0) != -1, "daClear should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daClear should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daClear should succeed if everything is okay");
    sput_run_test(testValid);

    sput_enter_suite("daClear should fail if we pass an unknown mode");
    sput_run_test(testUnknownMode);

    sput_enter_suite("daClear should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}