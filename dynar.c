#include "dynar.h"

/**
 * @brief The function reallocates the dynamic array to increase the space.
 *
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da The array that should be destroyed.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns 0 on success.
 * @returns Otherwise, -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
/*static int daRealloc(DaStruct *da, int *err) {
    return 0;
}*/

DaStruct *daCreate(DaDesc *desc, int *err)
{
    DaStruct *da;
    da = NULL;

    if (!err)
    {
        return NULL;
    }
    else if (!desc)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    if (desc->elements == 0 || desc->bytesPerElement == 0)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    da = calloc(1, sizeof(DaStruct) + (desc->elements * desc->bytesPerElement));

    if (!da)
    {
        goto err;
    }

    da->magic = DA_MAGIC;
    da->firstAddr = da + sizeof(DaStruct);
    da->lastAddr = da + sizeof(DaStruct) + (desc->elements * desc->bytesPerElement) - 1;
    da->freeAddr = da->firstAddr;
    da->used = 0;
    da->max = desc->elements;
    da->bytesPerElement = desc->bytesPerElement;

    *err = DA_OK;
    return da;

err:

    if (da)
    {
        da->magic = 0;
        free(da);
    }

    *err = DA_FATAL | DA_ENOMEM;
    return NULL;
}


int daDestroy(DaStruct *da, int *err)
{
    if (!err)
    {
        return -1;
    }
    else if (!da || da->magic != DA_MAGIC)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    memset(da, '0', sizeof(DaStruct) + (da->max * da->bytesPerElement));
    free(da);

    *err = DA_OK;
    return 0;
}


int daSize(DaStruct *da, int *err, size_t *size)
{
    if (!err)
    {
        return -1;
    }
    else if (!da || da->magic != DA_MAGIC || !size)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    *size = da->used;

    *err = DA_OK;
    return 0;
}


int daIsEmpty(DaStruct *da, int *err)
{
    if (!err)
    {
        return -1;
    }
    else if (!da || da->magic != DA_MAGIC)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    *err = DA_OK;
    return (da->used > 0) ? 0 : 1;
}

void *daGet(DaStruct *da, int *err, size_t pos)
{
    if (!err)
    {
        return NULL;
    }
    else if (!da || da->magic != DA_MAGIC)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    if (pos >= da->used)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return NULL;
    }

    *err = DA_OK;
    return ((char *)da->firstAddr + (pos * da->bytesPerElement));
}