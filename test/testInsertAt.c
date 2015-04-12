#include "sput.h"
#include "dynar.h"

static void testNull(void)
{
    int err;
    int ignore;
    DaStruct da;

    sput_fail_if(daInsertAt(NULL, NULL, &ignore, 0) != NULL, "daInsertAt(NULL, NULL, &ignore, 0) != NULL");
    sput_fail_if(daInsertAt(NULL, NULL, NULL, 0)    != NULL, "daInsertAt(NULL, NULL, NULL, 0)    != NULL");
    sput_fail_if(daInsertAt(&da,  NULL, &ignore, 0) != NULL, "daInsertAt(da,   NULL, &ignore, 0) != NULL");
    sput_fail_if(daInsertAt(&da,  NULL, NULL, 0)    != NULL, "daInsertAt(da,   NULL, NULL, 0)    != NULL");

    sput_fail_if(daInsertAt(&da,  &err, NULL, 0)    != NULL, "daInsertAt(da,   &err, NULL, 0)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daInsertAt(NULL, &err, &ignore, 0) != NULL, "daInsertAt(NULL, &err, &ignore, 0) != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");

    sput_fail_if(daInsertAt(NULL, &err, NULL, 0)    != NULL, "daInsertAt(NULL, &err, NULL, 0)    != NULL");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

/**
 * @brief The function inserts the @p element at @p pos.
 *
 * Shifts the element and any subsequent elements from @p pos one position to the right before inserting the new @p element.
 *
 * @p pos must be in between the array bounds.
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da      Insert the element into this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element The element that shall be inserted.
 * @param[in]  pos     Insert the @p element at this position into the array.
 *
 * @returns The function returns a pointer to the inserted element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da or @p element is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= daSize()).
 */
void *daInsertAt(DaStruct *da, int *err, const void *element, size_t pos);

static void testInsert(void)
{
    int err;
    void *freeAddrCmp;
    const char *src;
    const char *expect;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");
    memset(da->firstAddr, '0', 3);
    da->used = 1;

    src = "1";
    expect = "1";
    freeAddrCmp = da->freeAddr;
    sput_fail_if(daInsertAt(da, &err, src, 0) != da->firstAddr, "daInsertAt should return a pointer to the inserted element");
    sput_fail_if(memcmp(da->firstAddr, expect, 1) != 0, "daInsertAt should copy the element into the array (first position)");
    sput_fail_if(da->freeAddr != (char*)freeAddrCmp + 1, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->used != 2, "daInsertAt should increase the used counter");
    sput_fail_if(err != DA_OK, "err != DA_OK");


    src = "2";
    expect = "12";
    freeAddrCmp = da->freeAddr;
    sput_fail_if(daInsertAt(da, &err, src, 1) != (char *)da->firstAddr + 1, "daInsertAt should return a pointer to the inserted element");
    sput_fail_if(memcmp(da->firstAddr, expect, 2) != 0, "daInsertAt should copy the element into the array (first position)");
    sput_fail_if(da->freeAddr != (char*)freeAddrCmp + 1, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->used != 3, "daInsertAt should increase the used counter");
    sput_fail_if(err != DA_OK, "err != DA_OK");
}

static void testRealloc(void)
{
    int err;
    void *firstAddrCmp;
    void *LastAddrCmp;
    void *freeAddrCmp;
    const char *src;
    const char *expect;
    DaDesc desc;
    DaStruct *da;



    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "000";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;


    firstAddrCmp = da->firstAddr;
    LastAddrCmp = da->lastAddr;
    freeAddrCmp = da->freeAddr;
    src = "1";
    expect = "1000";
    sput_fail_if(daInsertAt(da, &err, src, 0) != da->firstAddr, "daInsertAt should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 4) != 0, "daInsertAt should copy the element to the second position");
    sput_fail_if(da->freeAddr != (char *)da->firstAddr + 4, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->used != 4, "daInsertAt should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer");
    sput_fail_if(da->lastAddr == LastAddrCmp, "daInsertAt should update the lastAddr pointer");
    sput_fail_if(da->freeAddr == freeAddrCmp, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->max != 6, "daInsertAt should double the maximum amount of elements");




    desc.elements = 3;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = "000";
    memcpy(da->firstAddr, src, 3);
    da->used = 3;

    firstAddrCmp = da->firstAddr;
    LastAddrCmp = da->lastAddr;
    freeAddrCmp = da->freeAddr;
    src = "1";
    expect = "0010";
    sput_fail_if(daInsertAt(da, &err, src, 2) != (char *)da->firstAddr + 2, "daInsertAt should return the address of the prepended element");
    sput_fail_if(err != DA_OK, "err != DA_OK");
    sput_fail_if(memcmp(da->firstAddr, expect, 4) != 0, "daInsertAt should copy the element to the second position");
    sput_fail_if(da->freeAddr != (char *)da->firstAddr + 4, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->used != 4, "daInsertAt should increase the used counter");
    sput_fail_if(da->firstAddr == firstAddrCmp, "daInsertAt should update the firstAddr pointer");
    sput_fail_if(da->lastAddr == LastAddrCmp, "daInsertAt should update the lastAddr pointer");
    sput_fail_if(da->freeAddr == freeAddrCmp, "daInsertAt should update the freeAddr pointer");
    sput_fail_if(da->max != 6, "daInsertAt should double the maximum amount of elements");
}

static void testBytesLimit(void)
{
    int err;
    char *src;
    DaDesc desc;
    DaStruct *da;

    desc.elements = 1;
    desc.bytesPerElement = DA_MAX_BYTES;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");

    src = malloc(DA_MAX_BYTES);
    sput_fail_if(src == NULL, "Unable to allocate space for the input");
    memset(src, '0', DA_MAX_BYTES);
    da->used = 0;


    sput_fail_if(daInsertAt(da, &err, src, 0) == NULL, "daInsertAt should succeed if the bytes limit isn't reached");
    sput_fail_if(err != DA_OK, "err != DA_OK");


    sput_fail_if(daInsertAt(da, &err, src, 0) != NULL, "daInsertAt should fail if the bytes limit is reached");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");




    desc.elements = DA_MAX_BYTES;
    desc.bytesPerElement = 1;
    da = daCreate(&desc, &err);
    sput_fail_if(da == NULL, "Unable to create dynamic array.");
    da->used = DA_MAX_BYTES;

    sput_fail_if(daInsertAt(da, &err, src, 0) != NULL, "daInsertAt should fail if the bytes limit is reached");
    sput_fail_if(err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT), "err != (DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT)");
}

static void testMagic(void)
{
    int ignore;
    int err;
    DaStruct da;

    da.magic = DA_MAGIC + 1;

    sput_fail_if(daInsertAt(&da, &err, &ignore, 0) != NULL, "daInsertAt should fail if the magic number mismatches");
    sput_fail_if(err != (DA_PARAM_ERR | DA_PARAM_NULL), "err != (DA_PARAM_ERR | DA_PARAM_NULL)");
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("daInsertAt should fail if any paramter is NULL");
    sput_run_test(testNull);

    sput_enter_suite("daInsertAt should insert an element");
    sput_run_test(testInsert);

    sput_enter_suite("daInsertAt should insert and realloc if necessary");
    sput_run_test(testRealloc);

    sput_enter_suite("daInsertAt shouldn't insert an element if the bytes limit is reached");
    sput_run_test(testBytesLimit);

    sput_enter_suite("daInsertAt should fail if the magic number in the header mismatches the expected magic number");
    sput_run_test(testMagic);

    sput_finish_testing();

    return sput_get_return_value();
}