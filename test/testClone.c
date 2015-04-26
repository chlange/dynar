#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daClone(NULL, NULL) != NULL, "daClone(NULL, NULL, 0) != NULL");
    sput_fail_if(daClone(&da,  NULL) != NULL, "daClone(&da,  NULL, 0) != NULL");
    sput_fail_if(daClone(NULL, &err) != NULL, "daClone(NULL, &err, 0) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testClone(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;
    DaStruct *clone;
    const char *src;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "123";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    clone = daClone(da, &err);
    sput_fail_if(clone == NULL, "daClone should succeed");
    sput_fail_if(clone->max != desc.elements, "daClone should copy the max counter");
    sput_fail_if(clone->bytesPerElement != desc.bytesPerElement, "daClone should copy the bytesPerElement counter");
    sput_fail_if(clone->used != da->used, "daClone should copy the used counter");
    sput_fail_if(memcmp(clone->firstAddr, src, 3) != 0, "daClone should copy the array content");
    sput_fail_if(clone->freeAddr != (char *)clone->firstAddr + 3, "daClone should set the freeAddr pointer");
    sput_fail_if(clone->magic != DA_MAGIC, "daClone should set the magic number");

    daDestroy(da, &err);
    daDestroy(clone, &err);
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daClone(&da, &err) != NULL, "daClone should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daClone should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daClone should succeed if everything is okay");
    sput_run_test(testClone);

    sput_enter_suite("daClone should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}