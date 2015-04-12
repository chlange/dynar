/*
 * Copyright (c) 2015, Christian Lange
 * (chlange) <Christian_Lange@hotmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Christian Lange nor the names of its
 *       contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CHRISTIAN LANGE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "dynar.h"

static int paramNotValid(const DaStruct *da, int *err);

/**
 * @brief The function reallocates the dynamic array to increase the space.
 *
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da  The array that should be reallocated.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns 0 on success.
 * @returns Otherwise, -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device.@n
 * ::DA_PARAM_ERR | ::DA_EXCEEDS_SIZE_LIMIT if the array can't be increased anymore. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
static int daRealloc(DaStruct *da, int *err)
{
    size_t bytes;
    size_t nrElements;
    void *newArray;

    if (paramNotValid(da, err))
    {
        return -1;
    }

    newArray = NULL;
    bytes = da->max * da->bytesPerElement;

    if (bytes == DA_MAX_BYTES)
    {
        *err = DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT;
        return -1;
    }
    else if (bytes <= (DA_MAX_BYTES / 2))
    {
        /* Enough room to double the array space */
        nrElements = da->max * 2;
    }
    else
    {
        /* Calculate maximum possible number of elements for this array */
        nrElements = DA_MAX_BYTES / da->bytesPerElement;
    }

    if (nrElements <= da->max || (nrElements * da->bytesPerElement) > DA_MAX_BYTES)
    {
        *err = DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT;
        return -1;
    }

    newArray = calloc(1, nrElements * da->bytesPerElement);

    if (!newArray)
    {
        *err = DA_FATAL | DA_ENOMEM;
        return -1;
    }

    memcpy(newArray, da->firstAddr, da->used * da->bytesPerElement);

    free(da->firstAddr);

    da->firstAddr = newArray;
    da->lastAddr = (char *)da->firstAddr + (nrElements * da->bytesPerElement) - 1;
    da->freeAddr = (char *)da->firstAddr + (da->used * da->bytesPerElement);
    da->max = nrElements;

    *err = DA_OK;
    return 0;
}

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
    else if (desc->elements * desc->bytesPerElement > DA_MAX_BYTES)
    {
        *err = DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT;
        return NULL;
    }

    da = calloc(1, sizeof(DaStruct));

    if (!da)
    {
        goto err;
    }

    da->magic = DA_MAGIC;
    da->firstAddr = calloc(1, desc->elements * desc->bytesPerElement);

    if (!da->firstAddr)
    {
        goto err;
    }

    da->lastAddr = (char *)da->firstAddr + (desc->elements * desc->bytesPerElement) - 1;
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
        free(da->firstAddr);
        free(da);
    }

    *err = DA_FATAL | DA_ENOMEM;
    return NULL;
}

int daDestroy(DaStruct *da, int *err)
{
    if (paramNotValid(da, err))
    {
        return -1;
    }

    da->magic = 0;
    memset(da->firstAddr, '0', da->max * da->bytesPerElement);
    free(da->firstAddr);
    free(da);

    *err = DA_OK;
    return 0;
}


int daSize(DaStruct *da, int *err, size_t *size)
{
    if (paramNotValid(da, err))
    {
        return -1;
    }
    else if (!size)
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
    if (paramNotValid(da, err))
    {
        return -1;
    }

    *err = DA_OK;
    return (da->used > 0) ? 0 : 1;
}

void *daGet(DaStruct *da, int *err, size_t pos)
{
    if (paramNotValid(da, err))
    {
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

void *daGetFirst(DaStruct *da, int *err)
{
    if (paramNotValid(da, err))
    {
        return NULL;
    }

    if (da->used == 0)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return NULL;
    }

    *err = DA_OK;
    return da->firstAddr;
}

void *daGetLast(DaStruct *da, int *err)
{
    if (paramNotValid(da, err))
    {
        return NULL;
    }

    if (da->used == 0)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return NULL;
    }

    *err = DA_OK;
    return ((char *)da->firstAddr + ((da->used - 1) * da->bytesPerElement));
}

int daClear(DaStruct *da, int *err, int mode)
{
    if (paramNotValid(da, err))
    {
        return -1;
    }

    switch (mode)
    {
        case DA_FAST:
            break;

        case DA_SECURE:
            memset(da->firstAddr, '0', da->used * da->bytesPerElement);
            break;

        default:
            *err = (DA_PARAM_ERR | DA_UNKNOWN_MODE);
            return -1;
            break;
    }

    da->freeAddr = da->firstAddr;
    da->used = 0;

    *err = DA_OK;
    return 0;
}

int daContains(DaStruct *da, int *err, const void *element)
{
    int i;
    int found;

    if (paramNotValid(da, err))
    {
        return -1;
    }
    else if (!element)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    *err = DA_NOT_FOUND;
    found = 0;

    for (i = 0; i < da->used; i++)
    {
        if (memcmp((char *)da->firstAddr + (i * da->bytesPerElement), element, da->bytesPerElement) == 0)
        {
            *err = DA_OK;
            found = 1;
            break;
        }
    }

    return found;
}

void *daSet(DaStruct *da, int *err, const void *element, size_t pos)
{
    if (paramNotValid(da, err))
    {
        return NULL;
    }
    else if (!element)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    if (pos >= da->used)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return NULL;
    }

    return memcpy((char *)da->firstAddr + (pos * da->bytesPerElement), element, da->bytesPerElement);
}

int daIndexOf(DaStruct *da, int *err, const void *element, size_t *index)
{
    size_t i;
    int found;

    if (paramNotValid(da, err))
    {
        return -1;
    }
    else if (!element || !index)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    *err = DA_NOT_FOUND;
    found = 0;

    for (i = 0; i < da->used; i++)
    {
        if (memcmp((char *)da->firstAddr + (i * da->bytesPerElement), element, da->bytesPerElement) == 0)
        {
            *index = i;
            *err = DA_OK;
            found = 1;
            break;
        }
    }

    return found;
}

int daLastIndexOf(DaStruct *da, int *err, const void *element, size_t *index)
{
    size_t i;
    int found;

    if (paramNotValid(da, err))
    {
        return -1;
    }
    else if (!element || !index)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return -1;
    }

    *err = DA_NOT_FOUND;
    found = 0;

    if (da->used > 0)
    {
        i = da->used;

        while (i--)
        {
            if (memcmp((char *)da->firstAddr + (i * da->bytesPerElement), element, da->bytesPerElement) == 0)
            {
                *index = i;
                *err = DA_OK;
                found = 1;
                break;
            }
        }
    }

    return found;
}

DaStruct *daClone(const DaStruct *da, int *err)
{
    DaDesc desc;
    DaStruct *clone;

    if (paramNotValid(da, err))
    {
        return NULL;
    }

    desc.elements = da->max;
    desc.bytesPerElement = da->bytesPerElement;

    clone = daCreate(&desc, err);

    if (clone)
    {
        clone->used = da->used;
        clone->freeAddr = (char *)clone->firstAddr + (clone->used * clone->bytesPerElement);
        memcpy(clone->firstAddr, da->firstAddr, clone->used * clone->bytesPerElement);
    }

    return clone;
}

int daRemove(DaStruct *da, int *err, size_t pos)
{
    void *dst;
    void *src;
    size_t bytes;

    if (paramNotValid(da, err))
    {
        return -1;
    }

    if (pos >= da->used)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return -1;
    }

    /* Move memory if it's not the last remaining or endmost element */
    if (da->used > 1 && pos < da->used - 1)
    {
        dst = (char *)da->firstAddr + (pos * da->bytesPerElement);
        src = (char *)dst + da->bytesPerElement;
        bytes = ((da->used - 1) - pos) * da->bytesPerElement;

        memmove(dst, src, bytes);
    }

    da->used--;
    da->freeAddr = (char *)da->freeAddr - da->bytesPerElement;

    *err = DA_OK;
    return 0;
}

void *daAppend(DaStruct *da, int *err, const void *element)
{
    void *ret;

    if (paramNotValid(da, err))
    {
        return NULL;
    }
    else if (!element)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    if (da->used == da->max)
    {
        if (daRealloc(da, err) != 0)
        {
            return NULL;
        }
    }

    ret = da->freeAddr;
    memcpy(da->freeAddr, element, da->bytesPerElement);
    da->used++;
    da->freeAddr = (char *)da->freeAddr + da->bytesPerElement;

    *err = DA_OK;
    return ret;
}
/**
 * @brief The function checks wheter the parameters are valid.
 *
 * The parameter @p da is valid if it's non-NULL and the magic number equals the defined .
 * @p err is valid if it's non-NULL.
 *
 * @param[in] da  Check this dynamic array
 * @param[in] err Check this pointer
 *
 * @returns The function returns 0 if the parameters are valid and -1 otherwise.
 */
static int paramNotValid(const DaStruct *da, int *err)
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

    return 0;
}