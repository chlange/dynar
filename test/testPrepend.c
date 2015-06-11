#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    int ignore;
    DaStruct da;

    memset(&da, '1', sizeof(da));

    sput_fail_if(daPrepend(NULL, NULL, &ignore) != NULL, "daPrepend(NULL, NULL, &ignore) != NULL");
    sput_fail_if(daPrepend(NULL, NULL, NULL)    != NULL, "daPrepend(NULL, NULL, NULL)    != NULL");
    sput_fail_if(daPrepend(&da,  NULL, &ignore) != NULL, "daPrepend(da,   NULL, &ignore) != NULL");
    sput_fail_if(daPrepend(&da,  NULL, NULL)    != NULL, "daPrepend(da,   NULL, NULL)    != NULL");

    sput_fail_if(daPrepend(&da,  &err, NULL)    != NULL, "daPrepend(da,   &err, NULL)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daPrepend(NULL, &err, &ignore) != NULL, "daPrepend(NULL, &err, &ignore) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daPrepend(NULL, &err, NULL)    != NULL, "daPrepend(NULL, &err, NULL)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testPrepend(void)
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

    src = "0";
    expect = "0";
    sput_fail_if(daPrepend(da, &err, src) != da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 1) != 0, "daPrepend should copy the element to the first position");
    sput_fail_if(da->used != 1, "daPrepend should increase the used counter");


    src = "1";
    expect = "10";
    sput_fail_if(daPrepend(da, &err, src) != da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 2) != 0, "daPrepend should copy the element to the first position");
    sput_fail_if(da->used != 2, "daPrepend should increase the used counter");


    src = "2";
    expect = "210";
    sput_fail_if(daPrepend(da, &err, src) !=  da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daPrepend should copy the element to the first position");
    sput_fail_if(da->used != 3, "daPrepend should increase the used counter");

    daDestroy(da, &err);
}

static void testRealloc(void)
{
    int err;
    void *firstAddrCmp;
    const char *src;
    const char *expect;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 10;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    src = "0";
    expect = "0";
    sput_fail_if(daPrepend(da, &err, src) != da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");




    firstAddrCmp = da->firstAddr;
    src = "1";
    expect = "10";
    sput_fail_if(daPrepend(da, &err, src) != da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 2) != 0, "daPrepend should copy the element to the second position");
    sput_fail_if(da->used != 2, "daPrepend should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daPrepend should update the firstAddr pointer");
    sput_fail_if(da->max != 2, "daPrepend should double the maximum amount of elements");




    firstAddrCmp = da->firstAddr;
    src = "2";
    expect = "210";
    sput_fail_if(daPrepend(da, &err, src) !=  da->firstAddr, "daPrepend should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daPrepend should copy the element to the third position");
    sput_fail_if(da->used != 3, "daPrepend should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daPrepend should update the firstAddr pointer");
    sput_fail_if(da->max != 4, "daPrepend should double the maximum amount of elements");

    daDestroy(da, &err);
}

static void testBytesLimit(void)
{
    int err;
    char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = malloc(desc.maxBytes);
    sput_fail_if(src == NULL, "Unable to allocate space for the input");
    memset(src, '0', desc.maxBytes);


    sput_fail_if(daPrepend(da, &err, src) == NULL, "daPrepend should succeed if the bytes limit isn't reached");
    sput_fail_if(err != DA_OK, "err != DA_OK");



    sput_fail_if(daPrepend(da, &err, src) != NULL, "daPrepend should fail if the bytes limit is reached");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");

    daDestroy(da, &err);


    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");
    da->used = 1;

    sput_fail_if(daPrepend(da, &err, src) != NULL, "daPrepend should fail if the bytes limit is reached");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");

    daDestroy(da, &err);
    free(src);
}

static void testMagic(void)
{
    int ignore;
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daPrepend(&da, &err, &ignore) != NULL, "daPrepend should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daPrepend should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daPrepend should prepend an element");
    sput_run_test(testPrepend);

    sput_enter_suite("daPrepend should prepend and realloc if necessary");
    sput_run_test(testRealloc);

    sput_enter_suite("daPrepend shouldn't prepend an element if the bytes limit is reached");
    sput_run_test(testBytesLimit);

    sput_enter_suite("daPrepend should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}