#ifndef __DYNAR_H__
#define __DYNAR_H__

#include <stddef.h>

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
#define DA_PARAM_ERR     0x10000000
/**
 * Parameter error: Indicates that a NULL-pointer was passed to a function.
 */
#define DA_PARAM_NULL    0x00000001
/**
 * Parameter error: Operation mode unknown.
 */
#define DA_MODE_UNKNOWN  0x00000002
/**
* Parameter error: Position out of array bounds.
*/
#define DA_OUT_OF_BOUNDS 0x00000003


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
#define DA_NOT_FOUND     0x30000000


/**
 * Magic number to avoid use-after-free or similar errors.
 */
#define DA_MAGIC         0x71238924

/**
 * Maximum number of elements in an array.
 */
#define DA_MAX_ELEMENTS      200000


/**
 * Operation mode for DaIncrease()
 */
#define DA_HARD          0x00000001

/**
 * Operation mode for DaIncrease()
 */
#define DA_SOFT          0x00000002

/**
 * @brief The structure defines the initial setup for an array.
 */
typedef struct __str_da_desc 
{
    /** 
     * Initial amount of elements for the array.
     */
	size_t elements;
    
    /**
     * Amount of bytes each element consists of.
     */
	size_t bytesPerElement;

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
     * Last address of the array.
     */
    void *lastAddr;

    /**
     * Next free address in the array.
     */
	void *freeAddr;

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
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
DaStruct *DaCreate(DaDesc *desc, int *err);

/**
 * @brief The function deletes a dynamic array.
 *
 * The function frees all allocated memory of the dynamic array @p da.
 * @p da gets invalid after successful execution.
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
int DaDestroy(DaStruct *da, int *err);

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
static int DaRealloc(DaStruct *da, int *err);

/**
 * @brief The function returns the number of elements in the array.
 *
 * @param[in]  da  Return the number of elements of this array. 
 * @param[out] err Indicates what went wrong in the event of an error.
 * 
 * @returns The function returns the number of elements of the array on success.
 * @returns Keep in mind to check the value of @p err.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
size_t DaSize(DaStruct *da, int *err);

/**
 * @brief The function returns the fill state of the array.
 *
 * @param[in]  da  Return the fill state of this array.
 * @param[out] err Indicates what went wrong in the event of an error.
 * 
 * @returns The function returns 1 if the array is empty or 0 if it's not empty.
 * @returns Keep in mind to check the value of @p err.
 *
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
int DaIsEmpty(DaStruct *da, int *err);

/**
 * @brief The function appends the @p element to the array.
 *
 * Shifts all elements in the array one position to the right before appending the new @p element.
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
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. 
 */
void *DaAppend(DaStruct *da, int *err, void *element);

/**
 * @brief The function prepends the @p element to the array.
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
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. 
 */
void *DaPrepend(DaStruct *da, int *err, void *element);

/**
 * @brief The function inserts the @p element at @p pos.
 *
 * Shifts the element and any subsequent elements from @p pos one position to the right before inserting the new @p element.
 * 
 * @p pos must be in between the array bounds.
 * The array remains unchanged in the event of an error.
 * 
 * @param[in]  da      Insert the element into to this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element The element that shall be inserted.
 * @param[in]  pos     Insert the @p element at this position in the array.
 *
 * @returns The function returns a pointer to the inserted element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= DaSize()).
 */
void *DaInsertAt(DaStruct *da, int *err, void *element, size_t pos);

/**
 * @brief The function deletes the element at @p pos.
 *
 * Shifts all subsequent elements from @p pos one position to the left.
 * 
 * @param[in]  da      Delete the element from this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  pos     Delete the @p element at this position from the array.
 * 
 * @returns Returns 0 on success, otherwise -1 is returned and @p err is set appropriately.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= DaSize()).
 */
int DaRemove(DaStruct *da, int *err, size_t pos);

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
void *DaGet(DaStruct *da, int *err, size_t pos);

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
void *DaGetFirst(DaStruct *da, int *err);

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
void *DaGetLast(DaStruct *da, int *err);

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
 * @param[in]  pos        Update the element at this position.
 *
 * @returns The function returns a pointer to the updated element on success.
 * @returns Otherwise, the function returns a NULL pointer and @p err is set appropriately.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if @p pos is out of the array bounds (@p pos >= DaSize()).
 */
void *DaSet(DaStruct *da, int *err, void *element, size_t pos);

/**
 * @brief The function increases the array in a way that @p n free slots for new elements are available.
 * 
 * The array remains unchanged in the event of an error.
 *
 * @param[in]  da   Prepend the element to this array.
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
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_MODE_UNKNOWN if @p mode is unknown.
 */
int DaIncrease(DaStruct *da, int *err, size_t n, int mode);

/**
 * @brief Removes all elements from the array.
 * 
 * @param[in]  da  Clear this array.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns 0 on success, otherwise, -1 is returned and @p err is set appropriately.
 * @returns In the case of an empty array 0 is returned.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
int DaClear(DaStruct *da, int *err);

/**
 * @brief Searches the array for the @p element.
 * 
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 * 
 * @returns Returns 1 if the array contains the @p element, otherwise 0 is returned.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if the array is empty.
 */
int DaContains(DaStruct *da, int *err, void *element);

/**
 * @brief Returns the index of the first occurence of the @p element in the array.
 * 
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 *
 * @returns Returns the index of the first occurence on success.
 * @returns Keep in mind to check the value of @p err.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_NOT_FOUND if the array doesn't contain an element equal to the specified @p element. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if the array is empty.
 */
size_t DaIndexOf(DaStruct *da, int *err, void *element);

/**
 * @brief Returns the index of the last occurence of the @p element in the array.
 * 
 * @param[in]  da      Search this array.
 * @param[out] err     Indicates what went wrong in the event of an error.
 * @param[in]  element Search the array for this element.
 *
 * @returns Returns the index of the last occurence on success.
 * @returns Keep in mind to check the value of @p err.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_NOT_FOUND if the array doesn't contain an element equal to the specified @p element. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 * ::DA_PARAM_ERR | ::DA_OUT_OF_BOUNDS if the array is empty.
 */
size_t DaLastIndexOf(DaStruct *da, int *err, void *element);

/**
 * @brief Returns a copy of the array.
 * 
 * @param[in]  da  Clone this array.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns a pointer to the cloned array on success, otherwise a NULL pointer is returned and @p is set appropriately.
 * 
 * @b Errors @n
 * ::DA_OK on success. @n
 * ::DA_FATAL | ::DA_ENOMEM if no space is left on device. @n
 * ::DA_PARAM_ERR | ::DA_PARAM_NULL if @p da is a NULL-pointer. @n
 */
DaStruct *DaClone(DaStruct *da, int *err);

#endif