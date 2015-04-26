#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    int ignore;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daContains(NULL, NULL, &ignore) != -1, "daContains(NULL, NULL, &ignore) != -1");
    sput_fail_if(daContains(NULL, NULL, NULL)    != -1, "daContains(NULL, NULL, NULL)    != -1");
    sput_fail_if(daContains(&da,  NULL, &ignore) != -1, "daContains(da,   NULL, &ignore) != -1");
    sput_fail_if(daContains(&da,  NULL, NULL)    != -1, "daContains(da,   NULL, NULL)    != -1");

    sput_fail_if(daContains(&da,  &err, NULL)    != -1, "daContains(da,   &err, NULL)    != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daContains(NULL, &err, &ignore) != -1, "daContains(NULL, &err, &ignore) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daContains(NULL, &err, NULL)    != -1, "daContains(NULL, &err, NULL)    != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testFound(void)
{
    int err;
    const char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    src = "1";
    sput_fail_if(daContains(da, &err, src) != 1, "daContains should find an existing element");
    sput_fail_if(err != DA_OK, "err != DA_OK");


    src = "2";
    sput_fail_if(daContains(da, &err, src) != 1, "daContains should find an existing element");
    sput_fail_if(err != DA_OK, "err != DA_OK");


    src = "3";
    sput_fail_if(daContains(da, &err, src) != 1, "daContains should find an existing element");
    sput_fail_if(err != DA_OK, "err != DA_OK");

    daDestroy(da, &err);
}

static void testNotFound(void)
{
    int err;
    const char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    src = "0";
    sput_fail_if(daContains(da, &err, src) != 0, "daContains shouldn't find an element that doesn't exist");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");

    src = "4";
    sput_fail_if(daContains(da, &err, src) != 0, "daContains shouldn't find an element that doesn't exist");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");


    da->used = 0;
    sput_fail_if(daContains(da, &err, src) != 0, "daContains shouldn't find an element if the array ie empty");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");

    daDestroy(da, &err);
}

static void testMagic(void)
{
    int ignore;
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daContains(&da, &err, &ignore) != -1, "daContains should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daContains should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daContains should find an existing element");
    sput_run_test(testFound);

    sput_enter_suite("daContains shouldn't find an element that doesn't exist");
    sput_run_test(testNotFound);

    sput_enter_suite("daContains should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}