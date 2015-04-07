#include "sput.h"
#include "dynar.c"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daRealloc(NULL, NULL) != -1, "daRealloc(NULL, NULL) != -1");
    sput_fail_if(daRealloc(&da,  NULL) != -1, "daRealloc(&da,  NULL) != -1");
    sput_fail_if(daRealloc(NULL, &err) != -1, "daRealloc(NULL, &err) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testExceedsSizeLimit(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;

    desc.elements = DA_MAX_BYTES;
    desc.bytesPerElement = 1;

    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    sput_fail_if(daRealloc(da, &err) != -1, "daRealloc should fail if it would exceed the size limit");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");
}

static void testValid(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;

    desc.elements = DA_MAX_BYTES / 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");
    sput_fail_if(daRealloc(da, &err) != 0, "daRealloc should succeed and double the space if it's far from limit");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != desc.elements * 2, "New maximum number should got doubled");

    sput_fail_if(daRealloc(da, &err) != 0, "daRealloc should succeed if we would step over the limit the first time and go to the maximum possible amount");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != DA_MAX_BYTES, "daRealloc should set the element space to max if doubling isn't possible anymore");

    sput_fail_if(daRealloc(da, &err) != -1, "daRealloc should fail if we can't increase the array anymore");
    sput_fail_if(da->max != DA_MAX_BYTES, "daRealloc shouldn't alter the maximum possible values if it's not possible to increase the space");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");



    desc.elements = DA_MAX_BYTES / 2;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(daRealloc(da, &err) != 0, "daRealloc should succeed if it would be near limit");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != DA_MAX_BYTES, "daRealloc should set the element space to max is doubling isn't possible anymore");

    sput_fail_if(daRealloc(da, &err) != -1, "daRealloc should fail if we can't increase the array anymore");
    sput_fail_if(da->max != DA_MAX_BYTES, "daRealloc shouldn't alter the maximum possible values if it's not possible to increase the space");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daRealloc(&da, &err) != -1, "daRealloc(...) should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daRealloc should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daRealloc shouldn't realloc if the array is too large");
    sput_run_test(testExceedsSizeLimit);

    sput_enter_suite("daRealloc should realloc if the array is small enough");
    sput_run_test(testValid);

    sput_enter_suite("daRealloc should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}