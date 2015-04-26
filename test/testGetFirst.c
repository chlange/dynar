#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daGetFirst(NULL, NULL)  != NULL, "daGetFirst(NULL, NULL) != NULL");
    sput_fail_if(daGetFirst(&da, NULL)   != NULL, "daGetFirst(&da,  NULL) != NULL");
    sput_fail_if(daGetFirst(NULL, &err)  != NULL, "daGetFirst(NULL, &err) != NULL");
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
    sput_fail_if(daGetFirst(da, &err) != da->firstAddr, "daGetFirst(...) should point to da->firstAddr");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da->used = 2;
    sput_fail_if(daGetFirst(da, &err) != da->firstAddr, "daGetFirst(...) should point to da->firstAddr");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    free(da);
}

static void testOutOfBounds(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daGetFirst(&da, &err) != NULL, "daGetFirst(...) should return NULL if the array is empty.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS), "err != (DA_PARAM_ERR | DA_OUT_OF_BOUNDS)");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daGetFirst(&da, &err) != NULL, "daGetFirst(...) should return NULL if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daGetFirst should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daGetFirst should succeed with valid values");
    sput_run_test(testValid);

    sput_enter_suite("daGetFirst should set err correctly if pos parameter is out of range");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daGetFirst should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}