#ifndef __DYNAR_H__
#define __DYNAR_H__

/** 
 * @file
 * @brief Header file for dynamic array.
 */

/**
 * Indicates that the operation was successful.
 */
#define DA_OK         0x00000000

/**
 * Group of parameter errors.
 * 
 * The actual parameter error gets bitwise OR'ed with this value.
 */
#define DA_PARAM_ERR  0x10000000
/**
 * Indicates that a NULL-pointer was passed to a function.
 */
#define DA_PARAM_NULL 0x00000001


/**
 * The structure defines the initial setup for 
 * an array.
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
 * The structure holds all relevant information
 * about the dynamic array.
 */
typedef struct __str_da_ptr
{
    /**
     * First address of array.
     */
	void *firstAddr;

	/**
     * Last address of array.
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
     * Amount of maximum elements in the array.
     */
	size_t max;

	/**
     * Amount of bytes each element consists of.
     */
    size_t bytesPerElement;

} DaPtr;

/**
 * @brief The function creates a new dynamic array.
 *
 * The function creates a dynamic array with the values given from @p desc.
 *
 * @param[in]  desc Initial settings for the database.
 * @param[out] err  Indicates what went wrong in the event of an error.
 * 
 * @returns A non-NULL pointer to the dynamic array on success that can be successfully passed to DaDestroy().
 * @returns Otherwise, a NULL pointer is returned and @p err gets set appropriately.
 * 
 * @returns @p err gets set to ::DA_OK if the operation was successful.
 * @returns @p err gets set to (::DA_PARAM_ERR|::DA_PARAM_NULL) if @p desc is a NULL-pointer.
 */
DaPtr *DaCreate(DaDesc *desc, int *err);

/**
 * @brief The function deletes a dynamic array.
 *
 * The function free's all allocated memory of the dynamic array \p da.
 * 
 * @param[in]  da The array that should be destroyed.
 * @param[out] err Indicates what went wrong in the event of an error.
 *
 * @returns Returns 0 on success.
 * @returns Otherwise, the function returns -1 and @p err gets set appropriately.
 * 
 * @returns @p err gets set to ::DA_OK if the operation was successful.
 * @returns @p err gets set to (::DA_PARAM_ERR|::DA_PARAM_NULL) if @p da is a NULL-pointer.
 */
int DaDestroy(DaPtr *da, int *err);

/* static int DaRealloc(DaPtr *desc, int *err) */

/**
 * @brief The function returns the number of elements currently used.
 *
 * @param[in]  da  Return the number of elements of this array. 
 * @param[out] err Indicates what went wrong in the event of an error.
 * 
 * @returns The function sets \p err to DA_OK and returns the number of elements of the array on success.
 * @returns Otherwise, @p err gets appropriately.
 *
 * @returns @p err gets set to ::DA_OK if the operation was successful.
 * @returns @p err gets set to (::DA_PARAM_ERR|::DA_PARAM_NULL) if @p da is a NULL-pointer.
 */
size_t DaSize(DaPtr *da, int *err);

/* 
 * The function returns the fill state.
 *
 * Returns:
 *	The function returns  1 if da is empty. 
 *  The function returns  0 if da is not empty.
 *  The function returns -1 if da is null.
 */
/* int DaIsEmpty(DaPtr *da, int *err) */

/* The function will append the element. */
/* The functions returns a non-NULL pointer that points to the top of the area that contains the element. */
/* void *DaAppend(DaPtr *da, int *err, void *element) */

/* The function will prepend the element. */
/* The functions returns a non-NULL pointer that points to the top of the area that contains the element. */
/* void *DaPrepend(DaPtr *da, int *err, void *element) */

/* The function will insert the element at pos. */
/* The functions returns a non-NULL pointer that points to the top of the area that contains the element. */
/* void *DaInsertAt(DaPtr *da, int *err, void *element, size_t pos) */

/* The function will remove the element at pos. */
/* int DaRemove(DaPtr *da, int *err, size_t pos) */

/* The function returns a non-NULL pointer to the element at pos. */
/* void *DaGet(DaPtr *da, int *err, size_t pos) */

/* The function replaces the element at pos with the newElement */
/* The function returns a non-NULL pointer to the element at pos. */
/* void *DaSet(DaPtr *da, int *err, void *newElement, size_t pos) */

/* 
 * The function increases the array in a way that n free slots for elements are available.
 *
 * mode can be either of the following:
 *   DaHard: Increase space for exactly n new elements no matter if enough space is available.
 *   DaSoft: Increase if not enough space for n new elements is available.
 */
/* DaIncrease(DaPtr *da, int *err, size_t n, int mode) */

#endif