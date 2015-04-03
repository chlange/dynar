#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daIsEmpty(NULL, NULL) != -1, "daIsEmpty(NULL, NULL) != -1");
    sput_fail_if(daIsEmpty(&da,  NULL) != -1, "daIsEmpty(da,   NULL) != -1");
    sput_fail_if(daIsEmpty(NULL, &err) != -1, "daIsEmpty(da,   NULL) != -1");

    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;
    da.used = 0;
    sput_fail_if(daIsEmpty(&da, &err) != 1, "daIsEmpty(...) should return 1 if array is empty");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 1;
    sput_fail_if(daIsEmpty(&da, &err) != 0, "daIsEmpty(...) should return 0 if array isn't empty");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 2;
    sput_fail_if(daIsEmpty(&da, &err) != 0, "daIsEmpty(...) should return 0 if array isn't empty");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daIsEmpty(&da, &err) != -1, "daIsEmpty(...) should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daIsEmpty should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daIsEmpty should return the fill state of the array");
    sput_run_test(testValid);

    sput_enter_suite("daIsEmpty should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}