#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    size_t idx;

    memset(&da, '1', sizeof(da));

    sput_fail_if(daIndexOf(&da, &err, &da, NULL)   != -1, "daIndexOf(&da, &err, &da, NULL)   != -1");
    sput_fail_if(daIndexOf(&da, &err, NULL, &idx)  != -1, "daIndexOf(&da, &err, NULL, &idx)  != -1");
    sput_fail_if(daIndexOf(&da, &err, NULL, NULL)  != -1, "daIndexOf(&da, &err, NULL, NULL)  != -1");

    sput_fail_if(daIndexOf(&da, NULL, &da, &idx)   != -1, "daIndexOf(&da, NULL, &da, &idx)   != -1");
    sput_fail_if(daIndexOf(&da, NULL, &da, NULL)   != -1, "daIndexOf(&da, NULL, &da, NULL)   != -1");
    sput_fail_if(daIndexOf(&da, NULL, NULL, &idx)  != -1, "daIndexOf(&da, NULL, NULL, &idx)  != -1");
    sput_fail_if(daIndexOf(&da, NULL, NULL, NULL)  != -1, "daIndexOf(&da, NULL, NULL, NULL)  != -1");

    sput_fail_if(daIndexOf(NULL, &err, &da, &idx)  != -1, "daIndexOf(NULL, &err, &da, &idx)  != -1");
    sput_fail_if(daIndexOf(NULL, &err, &da, NULL)  != -1, "daIndexOf(NULL, &err, &da, NULL)  != -1");
    sput_fail_if(daIndexOf(NULL, &err, NULL, &idx) != -1, "daIndexOf(NULL, &err, NULL, &idx) != -1");
    sput_fail_if(daIndexOf(NULL, &err, NULL, NULL) != -1, "daIndexOf(NULL, &err, NULL, NULL) != -1");

    sput_fail_if(daIndexOf(NULL, NULL, &da, &idx)  != -1, "daIndexOf(NULL, NULL, &da, &idx)  != -1");
    sput_fail_if(daIndexOf(NULL, NULL, &da, NULL)  != -1, "daIndexOf(NULL, NULL, &da, NULL)  != -1");
    sput_fail_if(daIndexOf(NULL, NULL, NULL, &idx) != -1, "daIndexOf(NULL, NULL, NULL, &idx) != -1");
    sput_fail_if(daIndexOf(NULL, NULL, NULL, NULL) != -1, "daIndexOf(NULL, NULL, NULL, NULL) != -1");

}

static void testFound(void)
{
    int err;
    size_t index;
    const char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    desc.maxBytes = 3;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    src = "1";
    sput_fail_if(daIndexOf(da, &err, src, &index) != 1, "daIndexOf should find the existing first element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(index != 0, "daIndexOf should return the index of the first element");


    src = "2";
    sput_fail_if(daIndexOf(da, &err, src, &index) != 1, "daIndexOf should find the existing second element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(index != 1, "daIndexOf should return the index of the second element");


    src = "3";
    sput_fail_if(daIndexOf(da, &err, src, &index) != 1, "daIndexOf should find the existing third element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(index != 2, "daIndexOf should return the index of the third element");

    daDestroy(da, &err);
}

static void testNotFound(void)
{
    int err;
    size_t index;
    const char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    desc.maxBytes = 3;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    src = "0";
    sput_fail_if(daIndexOf(da, &err, src, &index) != 0, "daIndexOf shouldn't find an element that doesn't exist");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");

    src = "4";
    sput_fail_if(daIndexOf(da, &err, src, &index) != 0, "daIndexOf shouldn't find an element that doesn't exist");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");

    da->used = 0;
    sput_fail_if(daIndexOf(da, &err, src, &index) != 0, "daIndexOf shouldn't find an element if the array is empty");
    sput_fail_if(err != DA_NOT_FOUND, "err != DA_NOT_FOUND");

    daDestroy(da, &err);
}

static void testMagic(void)
{
    size_t ignore;
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daIndexOf(&da, &err, &ignore, &ignore) != -1, "daIndexOf should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daIndexOf should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daIndexOf should find an existing element");
    sput_run_test(testFound);

    sput_enter_suite("daIndexOf shouldn't find an element that doesn't exist");
    sput_run_test(testNotFound);

    sput_enter_suite("daIndexOf should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}