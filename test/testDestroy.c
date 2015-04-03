#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daDestroy(NULL, NULL) != -1, "daDestroy(NULL, NULL) != -1");
    sput_fail_if(daDestroy(NULL, &err) != -1, "daDestroy(NULL, &err) != -1");
    sput_fail_if(daDestroy(&da,  NULL) != -1, "daDestroy(NULL, NULL) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testCleanup(void)
{
    int err;
    DaStruct da;

    /* Fake an array with 0 elements to avoid SIGSEGV
     * because daDestroy will memset the header and
     * (which would cause the segmentation fault) the elements to 0 */
    da.max = 0;
    sput_fail_if(daDestroy(&da, &err) != 0, "daDestroy(da, &err) != 0");
    sput_fail_if(err != DA_OK, "err != DA_OK.");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daDestroy should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daDestroy should clean up");
    sput_run_test(testCleanup);

    sput_finish_testing();

    return sput_get_return_value();
}