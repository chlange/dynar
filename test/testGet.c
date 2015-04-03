#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daGet(NULL, NULL, 0) != NULL, "daGet(NULL, NULL, 0) != NULL");
    sput_fail_if(daGet(&da,  NULL, 0) != NULL, "daGet(&da,  NULL, 0) != NULL");
    sput_fail_if(daGet(NULL, &err, 0) != NULL, "daGet(NULL, &err, 0) != NULL");
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
    da->max = 2;
    da->used = 2;
    da->bytesPerElement = 5;
    da->firstAddr = (char *)da + sizeof(DaStruct);

    sput_fail_if(daGet(da, &err, 0) != da->firstAddr, "daGet(da, &err, 0) should point to da->firstAddr");
    sput_fail_if(daGet(da, &err, 1) != (char *)da->firstAddr + da->bytesPerElement, "daGet(da, &err, 1) should point to second element");
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daGet(&da, &err, 0) != NULL, "daGet(&da, &err, 0) should return NULL if array is empty");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
    sput_fail_if(daGet(&da, &err, 1) != NULL, "daGet(&da, &err, 1) should return NULL if array is empty");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daGet(&da, &err, 0) != NULL, "daGet(...) should return NULL if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daGet should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daGet should succeed with valid values");
    sput_run_test(testValid);

    sput_enter_suite("daGet should set err correctly if pos parameter is out of range");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daGet should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}