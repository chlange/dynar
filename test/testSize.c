#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    size_t n;

    sput_fail_if(daSize(NULL, NULL, &n)   != -1, "daSize(NULL, NULL, &n)   != -1");
    sput_fail_if(daSize(NULL, NULL, NULL) != -1, "daSize(NULL, NULL, NULL) != -1");
    sput_fail_if(daSize(NULL, &err, &n)   != -1, "daSize(NULL, &err, &n)   != -1");
    sput_fail_if(daSize(NULL, &err, NULL) != -1, "daSize(NULL, &err, NULL) != -1");
    sput_fail_if(daSize(&da,  NULL, &n)   != -1, "daSize(da,   NULL, &n)   != -1");
    sput_fail_if(daSize(&da,  NULL, NULL) != -1, "daSize(da,   NULL, NULL) != -1");
    sput_fail_if(daSize(&da,  &err, NULL) != -1, "daSize(da,   &err, NULL) != -1");

    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
{
    int err;
    DaStruct da;
    size_t n;

    da.used = 0;
    sput_fail_if(daSize(&da, &err, &n) != 0, "daSize(da, &err, n) != 0");
    sput_fail_if(n != 0, "daSize returned an incorrect size.");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 10;
    sput_fail_if(daSize(&da, &err, &n) != 0, "daSize(da, &err, n) != 0");
    sput_fail_if(n != 10, "daSize returned an incorrect size.");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    da.used = 50;
    sput_fail_if(daSize(&da, &err, &n) != 0, "daSize(da, &err, n) != 0");
    sput_fail_if(n != 50, "daSize returned an incorrect size.");
    sput_fail_if(err != DA_OK, "err != DA_OK");

}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daSize should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daSize should return the size of the array");
    sput_run_test(testValid);

    sput_finish_testing();

    return sput_get_return_value();
}