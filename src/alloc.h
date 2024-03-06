/**
 * @file alloc.h
 * @brief Memory allocation functions.
 *
 * This file contains the declarations of memory allocation functions `alloc` and `dealloc`.
 * These functions are used to allocate and deallocate memory dynamically.
 */

#include <unistd.h>
#include <pthread.h>

/**
 * Allocates `size` bytes of memory, and returns a pointer to the allocated memory.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory, or NULL if the allocation fails.
 */
void *alloc(size_t size);

/**
 * Frees the memory space pointed to by `ptr`, which must have been returned by a previous call to `alloc`.
 *
 * @param ptr A pointer to the memory space to be freed.
 *
 * @return None.
 */
void dealloc(void *ptr);