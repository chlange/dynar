/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Christian Lange < Christian_Lange@hotmail.com >
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
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

    if (bytes == da->maxBytes)
    {
        *err = DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT;
        return -1;
    }
    else if (bytes <= (da->maxBytes / 2))
    {
        /* Enough room to double the array space */
        nrElements = da->max * 2;
    }
    else
    {
        /* Calculate maximum possible number of elements for this array */
        nrElements = da->maxBytes / da->bytesPerElement;
    }

    if (nrElements <= da->max || (nrElements * da->bytesPerElement) > da->maxBytes)
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

    if (desc->elements == 0 || desc->bytesPerElement == 0 || desc->maxBytes == 0)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }
    else if (desc->elements * desc->bytesPerElement > desc->maxBytes)
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

    da->used = 0;
    da->max = desc->elements;
    da->bytesPerElement = desc->bytesPerElement;
    da->maxBytes = desc->maxBytes;

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


size_t daSize(DaStruct *da, int *err)
{
    if (paramNotValid(da, err))
    {
        return 0;
    }

    *err = DA_OK;
    return da->used;
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

    da->used = 0;

    *err = DA_OK;
    return 0;
}

int daContains(DaStruct *da, int *err, const void *element)
{
    int i;
    int found;
    char *elementInArray;

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

    elementInArray = (char *)da->firstAddr;

    for (i = 0; i < da->used; i++)
    {
        if (memcmp(elementInArray, element, da->bytesPerElement) == 0)
        {
            *err = DA_OK;
            found = 1;
            break;
        }

        elementInArray += da->bytesPerElement;
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
    char *elementInArray;

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

    elementInArray = (char *)da->firstAddr;

    for (i = 0; i < da->used; i++)
    {
        if (memcmp(elementInArray, element, da->bytesPerElement) == 0)
        {
            *index = i;
            *err = DA_OK;
            found = 1;
            break;
        }

        elementInArray += da->bytesPerElement;
    }

    return found;
}

int daLastIndexOf(DaStruct *da, int *err, const void *element, size_t *index)
{
    size_t i;
    int found;
    char *elementInArray;

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

    /* Pointer to last element */
    elementInArray = (char *)da->firstAddr + ((da->used - 1) * da->bytesPerElement);

    if (da->used > 0)
    {
        i = da->used;

        while (i--)
        {
            if (memcmp(elementInArray, element, da->bytesPerElement) == 0)
            {
                *index = i;
                *err = DA_OK;
                found = 1;
                break;
            }

            elementInArray -= da->bytesPerElement;
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
    desc.maxBytes = da->maxBytes;

    clone = daCreate(&desc, err);

    if (clone)
    {
        clone->used = da->used;
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
        bytes = (da->used - 1 - pos) * da->bytesPerElement;

        memmove(dst, src, bytes);
    }

    da->used--;

    *err = DA_OK;
    return 0;
}

int daRemoveDirty(DaStruct *da, int *err, size_t pos)
{
    void *dst;
    void *src;

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
        src = (char *)da->firstAddr + ((da->used - 1) * da->bytesPerElement);

        memcpy(dst, src, da->bytesPerElement);
    }

    da->used--;

    *err = DA_OK;
    return 0;
}

int daRemoveRange(DaStruct *da, int *err, size_t from, size_t to)
{
    void *dst;
    void *src;
    size_t bytes;
    size_t elements;

    if (paramNotValid(da, err))
    {
        return -1;
    }

    if (to >= da->used || from >= da->used || from > to)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return -1;
    }

    elements = to - from;
    elements++;

    dst = (char *)da->firstAddr + (from * da->bytesPerElement);
    src = (char *)dst + (elements * da->bytesPerElement);
    bytes = (da->used - 1 - to) * da->bytesPerElement;

    memmove(dst, src, bytes);

    da->used -= elements;

    * err = DA_OK;
    return 0;

}

void *daAppend(DaStruct *da, int *err, const void *element)
{
    void *ret;
    void *freeAddr;

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

    ret = freeAddr = (char *)da->firstAddr + (da->used * da->bytesPerElement);
    memcpy(freeAddr, element, da->bytesPerElement);
    da->used++;

    *err = DA_OK;
    return ret;
}

void *daPrepend(DaStruct *da, int *err, const void *element)
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

    if (da->used == da->max)
    {
        if (daRealloc(da, err) != 0)
        {
            return NULL;
        }
    }

    memmove((char*)da->firstAddr + da->bytesPerElement, da->firstAddr, da->used * da->bytesPerElement);
    memcpy(da->firstAddr, element, da->bytesPerElement);
    da->used++;

    *err = DA_OK;
    return da->firstAddr;
}

void *daInsertAt(DaStruct *da, int *err, const void *element, size_t pos)
{
    void *dst;
    void *src;
    size_t bytes;

    if (paramNotValid(da, err))
    {
        return NULL;
    }
    else if (!element)
    {
        *err = DA_PARAM_ERR | DA_PARAM_NULL;
        return NULL;
    }

    if (pos > 0 && pos >= da->used)
    {
        *err = DA_PARAM_ERR | DA_OUT_OF_BOUNDS;
        return NULL;
    }

    if (da->used == da->max)
    {
        if (daRealloc(da, err) != 0)
        {
            return NULL;
        }
    }

    src = (char *)da->firstAddr + (pos * da->bytesPerElement);
    dst = (char *)src + da->bytesPerElement;
    bytes = (da->used - pos) * da->bytesPerElement;
    memmove(dst, src, bytes);
    memcpy(src, element, da->bytesPerElement);

    da->used++;

    *err = DA_OK;
    return src;
}

int daIncrease(DaStruct *da, int *err, size_t n, int mode)
{
    void *newArray;
    size_t freeSlots;
    size_t overflow;

    if (paramNotValid(da, err))
    {
        return -1;
    }

    freeSlots = da->max - da->used;
    overflow = 0;

    switch (mode)
    {
    case DA_SOFT:
        if (n > freeSlots)
        {
            overflow = n - freeSlots;
        }
        break;

    case DA_HARD:
        overflow = n;
        break;

    default:
        *err = DA_PARAM_ERR | DA_UNKNOWN_MODE;
        return -1;
        break;
    }

    if ((da->max + overflow) * da->bytesPerElement > da->maxBytes)
    {
        *err = DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT;
        return -1;
    }

    if (overflow > 0)
    {
        newArray = calloc(1, (da->max + overflow) * da->bytesPerElement);
        if (!newArray)
        {
            *err = DA_FATAL | DA_ENOMEM;
            return -1;
        }

        memcpy(newArray, da->firstAddr, da->used * da->bytesPerElement);

        free(da->firstAddr);

        da->firstAddr = newArray;
        da->max = da->max + overflow;
    }

    *err = DA_OK;
    return 0;
}

const char *daErrToString(int err)
{
    if (err == DA_OK)
    {
        return "DA_OK";
    }
    else if (err & DA_PARAM_ERR)
    {
        if (err & DA_PARAM_NULL)
        {
            return "DA_PARAM_ERR | DA_PARAM_NULL";
        }
        else if (err & DA_UNKNOWN_MODE)
        {
            return "DA_PARAM_ERR | DA_UNKNOWN_MODE";
        }
        else if (err & DA_OUT_OF_BOUNDS)
        {
            return "DA_PARAM_ERR | DA_OUT_OF_BOUNDS";
        }
        else if (err & DA_EXCEEDS_SIZE_LIMIT)
        {
            return "DA_PARAM_ERR | DA_EXCEEDS_SIZE_LIMIT";
        }
    }
    else if (err & DA_FATAL)
    {
        if (err & DA_ENOMEM)
        {
            return "DA_FATAL | DA_ENOMEM";
        }
    }
    else if (err == DA_NOT_FOUND)
    {
        return "DA_NOT_FOUND";
    }

    return "UNKNOWN ERROR";
}

int daDump(DaStruct *da, int *err)
{
    int i;
    int j;
    int ret;
    void *current;
    unsigned int addrLen;

    /* Number of digits to represent the index */
    static const unsigned int idxLen = 6;
    static const unsigned int addrMinWidth = 8;
    static const unsigned int hexMinWidth = 31;
    static const unsigned int asciiMinWidth = 5;
    static unsigned int hexLen;
    static unsigned int asciiLen;
    static char buf[50];

    if (paramNotValid(da, err))
    {
        return -1;
    }

    addrLen = sprintf(buf, "%p", da->firstAddr);
    if (addrLen < 0)
    {
        *err = DA_FATAL;
        return -1;
    }

    ret = 0;

    /* Each byte will be represented as a two digit hex number followed by one space to seperate bytes */
    hexLen = da->bytesPerElement * 3;
    asciiLen = da->bytesPerElement;

    addrLen  = (addrLen  < addrMinWidth ) ? addrMinWidth  : addrLen;
    hexLen   = (hexLen   < hexMinWidth  ) ? hexMinWidth   : hexLen;
    asciiLen = (asciiLen < asciiMinWidth) ? asciiMinWidth : asciiLen;

    printf("[ %*s ][ Index][%*s ][ %*s ]\n",  addrLen, "Address", hexLen, "Hexdump", asciiLen, "Ascii");

    for (i = 0; i < da->used; i++)
    {
        current = (char *)da->firstAddr + (i * da->bytesPerElement);

        /* Address and index */
        printf("[-%*p-][%*d]", addrLen , current, idxLen, i);

        /* Hexdump */
        printf("[ ");
        for (j = 0; j < da->bytesPerElement; j++)
        {
            ret = printf("%02x ", *((unsigned char *)current + j));
        }
        for (j = 0; j < hexLen - (ret * da->bytesPerElement); j++)
        {
            /* Padding */
            printf(" ");
        }
        printf("]");

        /* Ascii */
        printf("[ ");
        for (j = 0; j < da->bytesPerElement; j++)
        {
            if (isprint(*((char *)current + j)))
            {
                printf("%c", *((char *)current + j));
            }
            else
            {
                printf(".");
            }
        }
        for (j = 0; j < asciiLen - da->bytesPerElement; j++)
        {
            /* Padding */
            printf(" ");
        }
        printf(" ]");
        printf("\n");
    }

    if (da->used < da->max)
    {
        printf("(Skipped next %lu empty and unused elements)\n", (unsigned long)da->max - (unsigned long)da->used);
    }

    printf("\nDynamic array header\n");
    printf("--------------------\n");
    printf("da->firstAddr:   %10p\n", da->firstAddr);
    printf("da->used:        %10lu\n", (unsigned long)da->used);
    printf("da->max:         %10lu\n", (unsigned long)da->max);
    printf("da->bytesPerElement: %6lu\n", (unsigned long)da->bytesPerElement);

    printf("\n");

    *err = DA_OK;
    return 0;
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
