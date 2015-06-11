#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    DaStruct da;
    memset(&da, '1', sizeof(da));

    sput_fail_if(daIncrease(NULL, NULL, 0, 0) != -1, "daIncrease(NULL, NULL, 0, 0) != -1");
    sput_fail_if(daIncrease(&da,  NULL, 0, 0) != -1, "daIncrease(&da,  NULL, 0, 0) != -1");
    sput_fail_if(daIncrease(NULL, &err, 0, 0) != -1, "daIncrease(NULL, &err, 0, 0) != -1");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

static void testIncreaseSoft(void)
{
    int err;
    void *firstAddrCmp;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 100;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 0, DA_SOFT) != 0, "daIncrease should succeed but do nothing if the size parameter is 0 (DA_SOFT mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->firstAddr != firstAddrCmp, "daInsertAt shouldn't alter the firstAddr pointer if the size parameter is 0 (DA_SOFT mode)");
    sput_fail_if(da->max != 1, "daInsertAt shouldn't alter the max counter if the size parameter is 0 (DA_SOFT mode)");
    sput_fail_if(da->used != 0, "daInsertAt shouldn't alter the used counter if the size parameter is 0 (DA_SOFT mode)");


    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 1, DA_SOFT) != 0, "daIncrease shouldn't increase the array if enough space is available (DA_SOFT mode)");
    sput_fail_if(da->firstAddr != firstAddrCmp, "daInsertAt shouldn't update the firstAddr pointer if enough space is available (DA_SOFT mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 1, "daInsertAt shouldn't update the max counter if enough space is available (DA_SOFT mode)");



    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 2, DA_SOFT) != 0, "daIncrease should increase the array if not enough space is available (DA_SOFT mode)");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer if not enough space is available (DA_SOFT mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 2, "daInsertAt should update the max counter if not enough space is available (DA_SOFT mode)");



    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 10, DA_SOFT) != 0, "daIncrease should increase the array if not enough space is available (DA_SOFT mode)");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer if not enough space is available (DA_SOFT mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 10, "daInsertAt should update the max counter if not enough space is available (DA_SOFT mode)");

    daDestroy(da, &err);
}

static void testIncreaseHard(void)
{
    int err;
    void *firstAddrCmp;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 100;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 0, DA_HARD) != 0, "daIncrease should succeed but do nothing if the size parameter is 0 (DA_HARD mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->firstAddr != firstAddrCmp, "daInsertAt shouldn't alter the firstAddr pointer if the size parameter is 0 (DA_HARD mode)");
    sput_fail_if(da->max != 1, "daInsertAt shouldn't alter the max counter if the size parameter is 0 (DA_HARD mode)");
    sput_fail_if(da->used != 0, "daInsertAt shouldn't alter the used counter if the size parameter is 0 (DA_HARD mode)");


    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 1, DA_HARD) != 0, "daIncrease should increase the array (DA_HARD mode)");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer (DA_HARD mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 2, "daInsertAt should update the max counter (DA_HARD mode)");



    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 2, DA_HARD) != 0, "daIncrease should increase the array (DA_HARD mode)");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer (DA_HARD mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 4, "daInsertAt should update the max counter (DA_HARD mode)");



    firstAddrCmp = da->firstAddr;
    sput_fail_if(daIncrease(da, &err, 10, DA_HARD) != 0, "daIncrease should increase the array (DA_HARD mode)");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer (DA_HARD mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(da->max != 14, "daInsertAt should update the max counter (DA_HARD mode)");

    daDestroy(da, &err);
}

static void testNullSize(void)
{
    int err;
    DaStruct da;

    memset(&da, '0', sizeof(DaStruct));
    da.magic = DA_MAGIC;
    da.max = 10;
    da.used = 1;
    da.bytesPerElement = 1;
    da.maxBytes = 10;

    sput_fail_if(daIncrease(&da, &err, 0, DA_SOFT) != 0, "daIncrease should succeed if the size parameter is 0 (DA_SOFT mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");


    sput_fail_if(daIncrease(&da, &err, 0, DA_HARD) != 0, "daIncrease should succeed if the size parameter is 0 (DA_HARD mode)");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testUnknownMode(void)
{
    int err;
    DaStruct da;
    da.magic = DA_MAGIC;

    sput_fail_if(daIncrease(&da, &err, 0, 12345) != -1, "daIncrease should fail if the mode is unknown");
    sput_fail_if(err != (DA_PARAM_ERR | DA_UNKNOWN_MODE), "err != (DA_PARAM_ERR | DA_UNKNOWN_MODE)");

    sput_fail_if(daIncrease(&da, &err, 1, 12345) != -1, "daIncrease should fail if the mode is unknown");
    sput_fail_if(err != (DA_PARAM_ERR | DA_UNKNOWN_MODE), "err != (DA_PARAM_ERR | DA_UNKNOWN_MODE)");
}

static void testBytesLimit(void)
{
    int err;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = 1;
    desc.maxBytes = 1;

    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");


    sput_fail_if(daIncrease(da, &err, 2, DA_SOFT) != -1, "daIncrease should fail if the bytes limit would be reached in soft mode");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");


    sput_fail_if(daIncrease(da, &err, 2, DA_HARD) != -1, "daIncrease should fail if the bytes limit would be reached in hard mode");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");

    daDestroy(da, &err);
}

static void testMagic(void)
{
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daIncrease(&da, &err, 0, 0) != -1, "daIncrease should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daIncrease should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daIncrease should succeed if everything is okay in DA_SOFT mode");
    sput_run_test(testIncreaseSoft);

    sput_enter_suite("daIncrease should succeed if everything is okay in DA_HARD mode");
    sput_run_test(testIncreaseHard);

    sput_enter_suite("daIncrease should succeed if the size parameter is 0");
    sput_run_test(testNullSize);

    sput_enter_suite("daIncrease should return DA_UNKNOWN_MODE if the mode is unknown");
    sput_run_test(testUnknownMode);

    sput_enter_suite("daPrepend shouldn't increase the array if the bytes limit would be reached");
    sput_run_test(testBytesLimit);

    sput_enter_suite("daIncrease should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}