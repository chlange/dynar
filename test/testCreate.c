#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaDesc desc;

    desc.elements = 1;
    desc.bytesPerElement = 1;

    sput_fail_if(daCreate(NULL, NULL)  != NULL, "daCreate(NULL, NULL)  != NULL");
    sput_fail_if(daCreate(&desc, NULL) != NULL, "daCreate(&desc, NULL) != NULL");
    sput_fail_if(daCreate(NULL, &err)  != NULL, "daCreate(NULL, &err)  != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    desc.elements = 0;
    desc.bytesPerElement = 1;
    sput_fail_if(daCreate(&desc, &err)  != NULL, "daCreate(&desc, &err) with 0 elements != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    desc.elements = 1;
    desc.bytesPerElement = 0;
    sput_fail_if(daCreate(&desc, &err)  != NULL, "daCreate(&desc, &err) with 0 bytesPerElement != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 0;
    sput_fail_if(daCreate(&desc, &err)  != NULL, "daCreate(&desc, &err) with 0 maxBytes != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testBytesLimit(void)
{
    int err;
    DaDesc desc;

    desc.elements = 10;
    desc.bytesPerElement = 10;
    desc.maxBytes = 1;

    sput_fail_if(daCreate(&desc, &err)  != NULL, "daCreate(...) should return NULL if the byte limit gets exceeded.");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");
}

static void testInit(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 1;

    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "daCreate(&desc, &err) with valid values: fails.");
    sput_fail_if(err != DA_OK, "err != DA_OK.");
    sput_fail_if(da->firstAddr == NULL, "da->firstAddr == NULL");
    sput_fail_if(da->used != 0, "da->used != 0");
    sput_fail_if(da->max != desc.elements, "da->max != desc.elements");
    sput_fail_if(da->maxBytes != desc.maxBytes, "da->maxBytes != desc.maxBytes");
    sput_fail_if(da->bytesPerElement != desc.bytesPerElement, "da->bytesPerElement != desc.bytesPerElement");
    sput_fail_if(da->magic != DA_MAGIC, "da->magic != DA_MAGIC");

    daDestroy(da, &err);
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daCreate should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daCreate should fail the bytes limit get exceeded");
    sput_run_test(testBytesLimit);

    sput_enter_suite("daCreate should succeed with valid values");
    sput_run_test(testInit);

    sput_finish_testing();

    return sput_get_return_value();
}