#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

typedef struct __str_da_desc {
	/* size_t elements */
	/* size_t bytesPerElement */
} DaDesc;

typedef struct __str_da_ptr
{
	/* void *startCap */
	/* void *endCap */

	/* void *free */

	/* size_t used */
	/* size_t max */

	/* size_t bytesPerElement */
} DaPtr;

/* DaPtr *DaOpen(DaDesc *desc, int *err) */

/* void DaClose(DaPtr *desc, int *err) */

/* static int DaRealloc(DaPtr *desc, int *err) */

/*
 * The function returns the number of elements currently used.
 *
 * Returns:
 *	The function returns a value greater than or equal 0 on success.
 *  The function returns -1 if da is null.
 */
/* size_t DaSize(DaPtr *da) */

/* 
 * The function returns the fill state.
 *
 * Returns:
 *	The function returns  1 if da is empty. 
 *  The function returns  0 if da is not empty.
 *  The function returns -1 if da is null.
 */
/* int DaIsEmpty(DaPtr *da) */

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