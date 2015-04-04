#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daGetLast(NULL, NULL)  != NULL, "daGetLast(NULL, NULL) != NULL");
    sput_fail_if(daGetLast(&da, NULL)   != NULL, "daGetLast(&da,  NULL) != NULL");
    sput_fail_if(daGetLast(NULL, &err)  != NULL, "daGetLast(NULL, &err) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
{
    int err;
    DaStruct *da;

    /* Create an mock array with two elements each five bytes */
    da = malloc(sizeof(DaStruct) + (2 * 5));
    sput_fail_if(da == NULL, "No space left on device. Unable to test!");
    da->magic = DA_MAGIC;
    da->max = 1;
    da->bytesPerElement = 5;
    da->firstAddr = (char *)da + sizeof(DaStruct);

    da->used = 1;
    sput_fail_if(daGetLast(da, &err) != da->firstAddr, "daGetLast(...) should point to da->firstAddr");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da->used = 2;
    sput_fail_if(daGetLast(da, &err) != (char *)da->firstAddr + da->bytesPerElement, "daGetLast(...) should point to second element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daGetLast(&da, &err) != NULL, "daGetLast(...) should return NULL if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daGetLast(&da, &err) != NULL, "daGetLast(...) should return NULL if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daGetLast should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daGetLast should succeed with valid values");
    sput_run_test(testValid);

    sput_enter_suite("daGetLast should set err correctly if pos parameter is out of range");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daGetLast should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}