#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daSize(NULL, NULL) != 0, "daSize(NULL, NULL) != 0");
    sput_fail_if(daSize(&da,  NULL) != 0, "daSize(da,   NULL) != 0");

    sput_fail_if(daSize(NULL, &err)   != 0, "daSize(NULL, &err) != 0");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC;

    da.used = 0;
    sput_fail_if(daSize(&da, &err) != 0, "daSize(da, &err) should return 0");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 10;
    sput_fail_if(daSize(&da, &err) != 10, "daSize(da, &err) should return 10");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 50;
    sput_fail_if(daSize(&da, &err) != 50, "daSize(da, &err) should return 50");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daSize(&da, &err) != 0, "daSize(...) should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daSize should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daSize should return the size of the array");
    sput_run_test(testValid);

    sput_enter_suite("daSize should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}