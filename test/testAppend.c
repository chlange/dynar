#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    int ignore;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daAppend(NULL, NULL, &ignore) != NULL, "daAppend(NULL, NULL, &ignore) != NULL");
    sput_fail_if(daAppend(NULL, NULL, NULL)    != NULL, "daAppend(NULL, NULL, NULL)    != NULL");
    sput_fail_if(daAppend(&da,  NULL, &ignore) != NULL, "daAppend(da,   NULL, &ignore) != NULL");
    sput_fail_if(daAppend(&da,  NULL, NULL)    != NULL, "daAppend(da,   NULL, NULL)    != NULL");

    sput_fail_if(daAppend(&da,  &err, NULL)    != NULL, "daAppend(da,   &err, NULL)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daAppend(NULL, &err, &ignore) != NULL, "daAppend(NULL, &err, &ignore) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daAppend(NULL, &err, NULL)    != NULL, "daAppend(NULL, &err, NULL)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testAppend(void)
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

    src = "0";
    expect = "0";
    sput_fail_if(daAppend(da, &err, src) !=  (char *)da->firstAddr, "daAppend should return the address of the first element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 1) != 0, "daAppend should copy the element to the first position");
    sput_fail_if(da->used != 1, "daAppend should increase the used counter");


    src = "1";
    expect = "01";
    sput_fail_if(daAppend(da, &err, src) !=  (char *)da->firstAddr + 1, "daAppend should return the address of the second element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 2) != 0, "daAppend should copy the element to the second position");
    sput_fail_if(da->used != 2, "daAppend should increase the used counter");


    src = "2";
    expect = "012";
    sput_fail_if(daAppend(da, &err, src) !=  (char *)da->firstAddr + 2, "daAppend should return the address of the third element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daAppend should copy the element to the third position");
    sput_fail_if(da->used != 3, "daAppend should increase the used counter");


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
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    src = "0";
    expect = "0";
    sput_fail_if(daAppend(da, &err, src) != (char *)da->firstAddr, "daAppend should return the address of the appended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");




    firstAddrCmp = da->firstAddr;
    src = "1";
    expect = "01";
    sput_fail_if(daAppend(da, &err, src) != (char *)da->firstAddr + 1, "daAppend should return the address of the appended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 2) != 0, "daAppend should copy the element to the second position");
    sput_fail_if(da->used != 2, "daAppend should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daAppend should update the firstAddr pointer");
    sput_fail_if(da->max != 2, "daAppend should double the maximum amount of elements");




    firstAddrCmp = da->firstAddr;
    src = "2";
    expect = "012";
    sput_fail_if(daAppend(da, &err, src) !=  (char *)da->firstAddr + 2, "daAppend should append the third element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 3) != 0, "daAppend should copy the element to the third position");
    sput_fail_if(da->used != 3, "daAppend should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daAppend should update the firstAddr pointer");
    sput_fail_if(da->max != 4, "daAppend should double the maximum amount of elements");


    daDestroy(da, &err);
}

static void testBytesLimit(void)
{
    int err;
    char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = DA_MAX_BYTES;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = malloc(DA_MAX_BYTES);
    sput_fail_if(src == NULL, "Unable to allocate space for the input");
    memset(src, '0', DA_MAX_BYTES);


    sput_fail_if(daAppend(da, &err, src) == NULL, "daAppend should succeed if the bytes limit isn't reached");
    sput_fail_if(err != DA_OK, "err != DA_OK");



    sput_fail_if(daAppend(da, &err, src) != NULL, "daAppend should fail if the bytes limit is reached");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");

    daDestroy(da, &err);




    desc.elements = DA_MAX_BYTES;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");
    da->used = DA_MAX_BYTES;

    sput_fail_if(daAppend(da, &err, src) != NULL, "daAppend should fail if the bytes limit is reached");
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

    sput_fail_if(daAppend(&da, &err, &ignore) != NULL, "daAppend should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daAppend should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daAppend should append an element");
    sput_run_test(testAppend);

    sput_enter_suite("daAppend should append and realloc if necessary");
    sput_run_test(testRealloc);

    sput_enter_suite("daAppend shouldn't append an element if the bytes limit is reached");
    sput_run_test(testBytesLimit);

    sput_enter_suite("daAppend should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}