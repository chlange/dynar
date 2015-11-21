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

#ifndef __DYNAR_H__
#define __DYNAR_H__

#include <stdlib.h>
#include <string.h>
#ifdef INCLUDE_DUMP
#include <stdio.h>
#include <ctype.h>
#endif

/**
 * @file
 * @brief Header file for the dynamic array.
 *
 * The section @b Errors of each function description refers to the returned @p err value of the function.
 */

/**
 * Indicates that the operation was successful.
 */
#define DA_OK            0x00000000


/**
 * Group of parameter errors.
 *
 * The actual parameter error gets bitwise OR'ed with this value.
 */
#define DA_PARAM_ERR          0x10000000
/**
 * Parameter error: Indicates that a NULL-pointer was passed to a function.
 */
#define DA_PARAM_NULL         0x00000001
/**
 * Parameter error: Operation mode unknown.
 */
#define DA_UNKNOWN_MODE       0x00000002
/**
 * Parameter error: Position out of array bounds.
 */
#define DA_OUT_OF_BOUNDS      0x00000004
/**
 * Parameter error: Element size exceeds bytes limit.
 */
#define DA_EXCEEDS_SIZE_LIMIT 0x00000008

/**
 * Group of fatal errors.
 *
 * The actual parameter error gets bitwise OR'ed with this value.
 */
#define DA_FATAL         0x20000000
/**
 * Fatal error: No space left on device.
 */
#define DA_ENOMEM        0x00000001

/**
 * Element not found in array.
 */
#define DA_NOT_FOUND     0x40000000


/**
 * Magic number to avoid use-after-free or similar errors.
 */
#define DA_MAGIC         0x71238924


/**
 * Operation mode for daIncrease()
 */
#define DA_HARD          0x00000001
/**
 * Operation mode for daIncrease()
 */
#define DA_SOFT          0x00000002


/**
 * Operation mode for daClear()
 */
#define DA_FAST          0x00000001
/**
 * Operation mode for daClear()
 */
#define DA_SECURE        0x00000002

/**
 * @brief The structure defines the initial setup for an array.
 */
typedef struct __str_da_desc
{
    /**
     * Initial amount of elements for the array.
     * Must be greater than 0.
     */
    size_t elements;

    /**
     * Amount of bytes each element consists of.
     * Must be greater than 0.
     */
    size_t bytesPerElement;

    /**
     * Overall byte limit for the array.
     * Must be greater than 0.
     */
    size_t maxBytes;

} DaDesc;

/**
 * @brief The structure holds all relevant information about the dynamic array.
 */
typedef struct __str_da_ptr
{
    /**
     * First address of the array.
     */
    void *firstAddr;

    /**
     * Amount of elements currently used.
     */
    size_t used;

    /**
     * Amount of maximum elements in the current array.
     */
    size_t max;

    /**
     * Amount of bytes each element consists of.
     */
    size_t bytesPerElement;

    /**
     * Overall byte limit of the array.
     */
    size_t maxBytes;

    /**
     * Magic number to avoid use-after-free or similar errors.
     *
     * Gets set to ::DA_MAGIC when the array gets created and set to 0 when it gets destroyed.
     */
    int magic;

} DaStruct;

/**
 * @brief The function creates a new dynamic array.
 *
 * It allocates memory for the initial settings given by @p desc and initializes all
 * values in the newly created ::DaStruct object.
 *
 * @param[in]  desc Initial settings for the array.
 * @param[out] err  Indicates what went wrong in the event of an error.
 *
 * @returns Returns a pointer to the dynamic array on success that can be successfully passed to DaDestroy().
 * @returns Otherwise, a NULL pointer is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device.@n
 * ::DA_PARAM_ERR | ::DA_EXCEEDS_SIZE_LIMIT if the settings given by @p desc exceed the bytes limit DaDesc#maxBytes. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p desc is a NULL-pointer. @n
 */
DaStruct *daCreate(DaDesc *desc, int *err);

/**
 * @brief The function deletes a dynamic array.
 *
 * The function frees all allocated memory of the dynamic array @p da.
 * @p da gets invalid after successful execution.
 *
 * @param[in]  da  The array that should be destroyed.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns 0 on success.
 * @returns Otherwise, -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
int daDestroy(DaStruct *da, int *err);

/**
 * @brief The function returns the number of elements in the array.
 *
 * @param[in]  da   Return the number of elements of this array.
 * @param[out] err  Indicates what went wrong in the event of an error.
 *
 * @returns The function returns the number of elements in the array on success.
 * @returns Otherwise, the function returns 0 and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
size_t daSize(DaStruct *da, int *err);

/**
 * @brief The function returns the fill state of the array.
 *
 * @param[in]  da  Return the fill state of this array.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns The function returns 1 if the array is empty or 0 if it's not empty.
 * @returns In the event of an error the function returns -1 and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
int daIsEmpty(DaStruct *da, int *err);

/**
 * @brief The function prepends the @p element to the array.
 *
 * It shifts all elements in the array one position to the right before prepending the new @p element.
 * The function copies the bytes of @p element to the free slot.
 *
 * The array will be increased if necessary.
 * It remains unchanged in the event of an error.
 *
 * @param[in]  da      Prepend the element to this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element The element that shall be prepended.
 *
 * @returns The function returns a pointer to the prepended element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device.@n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da or @p element is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_EXCEEDS_SIZE_LIMIT if the reallocation whould exceed the bytes limit DaStruct#maxBytes.
 */
void *daPrepend(DaStruct *da, int *err, const void *element);

/**
 * @brief The function appends the @p element to the array.
 *
 * It copies the bytes of @p element to the end of the array.
 *
 * The array will be increased if necessary.
 * It remains unchanged in the event of an error.
 *
 * @param[in]  da      Append the element to this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element The element that shall be appended.
 *
 * @returns The function returns a pointer to the appended element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device.@n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da or @p element is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_EXCEEDS_SIZE_LIMIT if the reallocation whould exceed the bytes limit DaStruct#maxBytes.
 */
void *daAppend(DaStruct *da, int *err, const void *element);

/**
 * @brief The function inserts the @p element at @p pos.
 *
 * It shifts the element and any subsequent elements from @p pos one position to the right before inserting the new @p element.
 *
 * The function copies the bytes of @p element to @p pos.
 *
 * It's allowed to use 0 for @p pos if the array is empty to append the element.
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

/**
 * @brief The function deletes the element at @p pos.
 *
 * Shifts all subsequent elements from @p pos one position to the left.
 *
 * @p pos must be in between the array bounds.
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da      Delete the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  pos     Delete the @p element at this position from the array (0 based).
 *
 * @returns Returns 0 on success, otherwise -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= daSize()).
 */
int daRemove(DaStruct *da, int *err, size_t pos);

/**
 * @brief The function deletes the element at @p pos in a quick way.
 *
 * It will copy the last element to pos and is therefore faster than daRemove().
 * But the array order doens't remain the same.
 *
 * @p pos must be in between the array bounds.
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da      Delete the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  pos     Delete the @p element at this position from the array (0 based).
 *
 * @returns Returns 0 on success, otherwise -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= daSize()).
 */
int daRemoveDirty(DaStruct *da, int *err, size_t pos);

/**
 * @brief The function removes a range of elements.
 *
 * It will remove all elements between (inclusive) @p from and (inclusive) @p to.
 *
 * @param[in]  da   Delete the elements from this array.
 * @param[out] err  Indicates what went wrong in the event of an error.
 * @param[in]  from Start to remove elements starting at this index (0 based).
 * @param[in]  to   Last index of element that should be deleted (0 based).
 *
 * @return Returns 0 on success, otherwise -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if any index is out of bounds (@p to > daSize() or @p from > daSize() or @p from > @p to).
 */
int daRemoveRange(DaStruct *da, int *err, size_t from, size_t to);

/**
 * @brief The function returns the element at @p pos.
 *
 * @param[in]  da      Get the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  pos     Get the @p element at this position from the array.
 *
 * @returns The function returns a pointer to the element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= DaSize()).
 */
void *daGet(DaStruct *da, int *err, size_t pos);

/**
 * @brief The function returns the first element of the array
 *
 * @param[in]  da      Get the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 *
 * @returns The function returns a pointer to the first element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if the array is empty.
 */
void *daGetFirst(DaStruct *da, int *err);

/**
 * @brief The function returns the last element of the array
 *
 * @param[in]  da      Get the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 *
 * @returns The function returns a pointer to the last element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if the array is empty.
 */
void *daGetLast(DaStruct *da, int *err);

/**
 * @brief The function replaces the element at @p pos with the contents from the specified @p element.
 *
 * The function copies the bytes from the specified @p element to the element at @p pos.
 *
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da         Update the element of this array.
 * @param[out] err        Indicates what went wrong in the event of an error.
 * @param[in]  element    Replace the current element with this element.
 * @param[in]  pos        Update the element at this position (0 based).
 *
 * @returns The function returns a pointer to the updated element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da or @p element is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= DaSize()).
 */
void *daSet(DaStruct *da, int *err, const void *element, size_t pos);

/**
 * @brief The function increases the array in a way that @p n free slots for new elements are available.
 *
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da   Increase this array.
 * @param[out] err  Indicates what went wrong in the event of an error.
 * @param[in]  n    Increase array by this amount.
 * @param[in]  mode Specifies the operation mode of the function.
 *
 * @b Modes @n
 *   @p ::DA_HARD: Increase space for exactly @p n new elements no matter if enough space is available.@n
 *   @p ::DA_SOFT: Increase the array only if not enough space for @p n new elements is available.
 *
 * @returns Returns 0 on success, otherwise, -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_EXCEEDS_SIZE_LIMIT if the a reallocation whould exceed the bytes limit DaStruct#maxBytes. @n
 * ::DA_PARAM_ERR | ::DA_UNKNOWN_MODE if @p mode is unknown.
 */
int daIncrease(DaStruct *da, int *err, size_t n, int mode);

/**
 * @brief Removes all elements from the array.
 *
 * @param[in]  da   Clear this array.
 * @param[out] err  Indicates what went wrong in the event of an error.
 * @param[in]  mode Specifies the operation mode of the function.
 *
 * @b Modes @n
 *   @p ::DA_FAST:   The fast mode will free all slots in the array but won't erase the content of the elements.@n
 *   @p ::DA_SECURE: The secure mode erases the content of the array before clearing it.
 *
 * @returns Returns 0 on success, otherwise, -1 is returned and @p err is set appropriately.
 * @returns In the case of an already empty array 0 is returned.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_UNKNOWN_MODE if @p mode is unknown.
 */
int daClear(DaStruct *da, int *err, int mode);

/**
 * @brief Checks whether the array contains the @p element.
 *
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 *
 * @returns Returns 1 if the array contains the @p element and 0 if not.
 * @returns The function returns -1 in the event of an error and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_NOT_FOUND if the array doesn't contain an element equal to the specified @p element (e.g. the array is empty). @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da or @p element is a NULL-pointer. @n
 */
int daContains(DaStruct *da, int *err, const void *element);

/**
 * @brief Returns the index of the first occurence of the @p element in the array.
 *
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 * @param[out] index   Index of the @p element if the array contains it (0 based).
 *
 * @returns Returns  1 if the array contains the @p element and the index is returned via @p index.
 * @returns Returns  0 if the array doesn't contain the @p element.
 * @returns Returns -1 in the event of an error and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_NOT_FOUND if the array doesn't contain an element equal to the specified @p element (e.g. the array is empty). @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da, @p element or @p index is a NULL-pointer.
 */
int daIndexOf(DaStruct *da, int *err, const void *element, size_t *index);

/**
 * @brief Returns the index of the last occurence of the @p element in the array.
 *
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 * @param[out] index   Index of the @p element if the array contains it (0 based).
 *
 * @returns Returns  1 if the array contains the @p element and the index is returned via @p index.
 * @returns Returns  0 if the array doesn't contain the @p element.
 * @returns Returns -1 in the event of an error and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_NOT_FOUND if the array doesn't contain an element equal to the specified @p element (e.g. the array is empty). @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da, @p element or @p index is a NULL-pointer.
 */
int daLastIndexOf(DaStruct *da, int *err, const void *element, size_t *index);

/**
 * @brief Returns a copy of the array.
 *
 * The initial array @p da remains unchanged.
 *
 * @param[in]  da  Clone this array.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns a pointer to the cloned array on success, otherwise a NULL pointer is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
DaStruct *daClone(const DaStruct *da, int *err);

/**
 * @brief Returns the number encoded error code as a string.
 *
 * @param[in] err Translate this error code into the appriopriate string.
 *
 * @returns Returns a human-readable error string as seen in the function descriptions Errors section.
 */
const char *daErrToString(int err);

#ifdef INCLUDE_DUMP
/**
 * @brief The function prints the header and array of the dynamic array pointed to by @p da to stdout.
 *
 * @param da Print this dynamic array.
 *
 * @returns The function returns 0 on success, otherwise -1 is returned and @p err is set appropriately.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL if a fatal error occurs during dump. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
int daDump(DaStruct *da, int *err);
#endif

#endif
