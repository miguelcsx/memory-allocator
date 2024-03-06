/**
 * @file main.c
 * @brief Memory Allocator Example
 *
 * This file contains an example program that demonstrates the usage of a memory allocator.
 * It allocates memory for strings, arrays of integers, and large arrays, and then deallocates the memory.
 * The program also handles edge cases such as allocating zero bytes and deallocating NULL pointers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"

/**
 * Prints an error message to the standard error stream and exits the program.
 *
 * @param message The error message to be printed.
 */
void fail(char *message) {
    fprintf(stderr, "%s", message);
    exit(1);
}

int main() {
    printf("Allocating memory for a string\n");
    // Allocate memory for a string
    char *str1 = (char *)alloc(20 * sizeof(char));
    if (str1 == NULL) {
        fail("Failed to allocate memory for string\n");
    }
     // Copy string to allocated memory
    strcpy(str1, "Memory Allocator");
    printf("String: %s\n", str1);

    printf("\nAllocating memory for an array of integers\n");
    // Allocate memory for an array of integers
    int *int_array = (int *)alloc(5 * sizeof(int));
    if (int_array == NULL) {
        fail("Failed to allocate memory for integer array\n");
    }
    for (int i = 0; i < 5; i++) {
        // Initialize array elements
        int_array[i] = i * 2; 
    }
    printf("Integer array: ");
    for (int i = 0; i < 5; i++) {
        // Print array elements
        printf("%d ", int_array[i]);
    }
    printf("\n");

    printf("\nDeallocating memory for string\n");
    // Deallocate memory for string
    dealloc(str1);

    printf("Allocating memory for another string, should reuse the previously deallocated memory\n");
    // Reallocate memory for string
    str1 = (char *)alloc(20 * sizeof(char));
    printf("Returned address: %p\n", str1);

    printf("\nDeallocating memory for integer array\n");
    // Deallocate memory for integer array
    dealloc(int_array);

    printf("Allocating memory for a large array, may require additional memory allocation\n");
    // Allocate memory for a large array
    long *large_array = (long *)alloc(100000 * sizeof(long));
    if (large_array == NULL) {
        fail("Failed to allocate memory for large array\n");
    }
    printf("Returned address: %p\n", large_array);

    printf("\nDeallocating memory for large array\n");
    // Deallocate memory for large array
    dealloc(large_array);

    printf("Deallocating memory for second string\n");
    // Deallocate memory for second string
    dealloc(str1);

    printf("Allocating zero bytes, should return a non-NULL pointer\n");
    // Allocate zero bytes of memory
    void *zero_ptr = alloc(0);
    if (zero_ptr == NULL) {
        printf("Malloc with size 0 returned NULL\n");
    } else {
        printf("Malloc with size 0 returned a non-NULL pointer: %p\n", zero_ptr);
        // Deallocate zero bytes of memory
        dealloc(zero_ptr);
    }

    printf("\nDeallocating NULL pointer (should not crash)\n");
    // Deallocate NULL pointer (no operation)
    dealloc(NULL);

    return 0;
}
