#include "dynar.h"
#include "sput.h"

static void testNull(void)
{
    int err;
    DaStruct da;

    sput_fail_if(daDestroy(NULL, NULL) != -1, "daDestroy(NULL, NULL) != -1");
    sput_fail_if(daDestroy(NULL, &err) != -1, "daDestroy(NULL, &err) != -1");
    sput_fail_if(daDestroy(&da,  NULL) != -1, "daDestroy(&da,  NULL) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testCleanup(void)
{
    int err;
    DaStruct *da;

    da = malloc(sizeof(DaStruct));
    sput_fail_if(da == NULL, "No space left on device. Unable to test!");

    /* Fake an array with 0 elements to avoid SIGSEGV
     * because daDestroy will memset the header and
     * (which would cause the segmentation fault) the elements to 0 */
    da->max = 0;
    da->magic = DA_MAGIC;
    sput_fail_if(daDestroy(da, &err) != 0, "daDestroy(da, &err) should return 0 after successful cleanup");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC - 1;

    sput_fail_if(daDestroy(&da, &err) != -1, "daDestroy(...) should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daDestroy should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daDestroy should clean up");
    sput_run_test(testCleanup);

    sput_enter_suite("daDestroy should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}