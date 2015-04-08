#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    int ignore;
    DaStruct da;

    sput_fail_if(daSet(NULL, NULL, &ignore, 0) != NULL, "daSet(NULL, NULL, &ignore, 0) != NULL");
    sput_fail_if(daSet(NULL, NULL, NULL, 0)    != NULL, "daSet(NULL, NULL, NULL, 0)    != NULL");
    sput_fail_if(daSet(&da,  NULL, &ignore, 0) != NULL, "daSet(da,   NULL, &ignore, 0) != NULL");
    sput_fail_if(daSet(&da,  NULL, NULL, 0)    != NULL, "daSet(da,   NULL, NULL, 0)    != NULL");

    sput_fail_if(daSet(&da,  &err, NULL, 0)    != NULL, "daSet(da,   &err, NULL, 0)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daSet(NULL, &err, &ignore, 0) != NULL, "daSet(NULL, &err, &ignore, 0) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daSet(NULL, &err, NULL, 0)    != NULL, "daSet(NULL, &err, NULL, 0)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testValid(void)
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

    src = "523";
    sput_fail_if(daSet(da, &err, src, 0) != (char *)da->firstAddr + 0, "daSet should replace the existing element and return a pointer to the element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, src, 3) != 0, "daSet didn't update the element at position 0");

    src = "553";
    sput_fail_if(daSet(da, &err, src, 1) != (char *)da->firstAddr + 1, "daSet should replace the existing element and return a pointer to the element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, src, 3) != 0, "daSet didn't update the element at position 1");

    src = "555";
    sput_fail_if(daSet(da, &err, src, 2) != (char *)da->firstAddr + 2, "daSet should replace the existing element and return a pointer to the element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, src, 3) != 0, "daSet didn't update the element at position 2");
}

static void testOutOfBounds(void)
{
    int err;
    int ignore;
    DaStruct da;

    da.used = 1;
    sput_fail_if(daSet(&da, &err, &ignore, 1) != NULL, "daSet should fail if out of bounds");
    sput_fail_if(daSet(&da, &err, &ignore, 10) != NULL, "daSet should fail if out of bounds");
}

static void testMagic(void)
{
    int err;
    int ignore;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daSet(&da, &err, &ignore, 0) != NULL, "daSet should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daSet should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daSet should replace the existing element");
    sput_run_test(testValid);

    sput_enter_suite("daSet shouldn't find an element that doesn't exist");
    sput_run_test(testOutOfBounds);

    sput_enter_suite("daSet should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}