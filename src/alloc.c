/**
 * @file alloc.c
 * @brief Implementation of a memory allocator.
 *
 * This file contains the implementation of a memory allocator, which manages
 * memory chunks using a linked list data structure. The memory chunks are
 * represented by the `chunk_t` structure, which contains the size of the chunk
 * and a pointer to the next chunk in the list.
 */

#include "alloc.h"

// Metadata structure for memory chunks
typedef struct chunk {
	size_t size;          // Size of the memory chunk
	struct chunk *next;   // Pointer to the next chunk in the list
} chunk_t;

// Head of the free list
static chunk_t *free_list = NULL;

// Last allocated/deallocated chunk (needed for the next fit algorithm)
static chunk_t *last_chunk = NULL;

// Mutex for synchronization
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Initializes the memory pool with the specified initial size.
 *
 * @param initial_size The initial size of the memory pool.
 */
void init_memory_pool(size_t initial_size) {
	// Use sbrk to allocate a large chunk of memory
	// Sbrk is easy to use, but not the most efficient way to allocate memory
	void *memory_pool = sbrk(initial_size);

	// Setup the initial chunck and add it to the free list
	chunk_t *initial_chunk = (chunk_t *) memory_pool;
	initial_chunk->size = initial_size;
	initial_chunk->next = NULL;
	// Set the free list and last chunk
	free_list = last_chunk = initial_chunk;
}

void *alloc(size_t size) {
	// Lock before accessing shared resources
	pthread_mutex_lock(&mutex);

	// Search the free list for a suitable chunk (next fit approach)
	chunk_t *chunk = last_chunk;
	chunk_t *prev_chunk = NULL;

	// Adjust the size to account for metadata overhead and alignment
	size_t adjusted_size = size + sizeof(chunk_t);
	adjusted_size = (adjusted_size + 7) & ~7; // Align to 8 bytes

	// Traverse the free list to find a suitable chunk
	while (chunk != NULL) {
		if (chunk->size >= adjusted_size) {
			// Found a suitable chunk
			break;
		}
		// Move to the next chunk
		prev_chunk = chunk;
		chunk = chunk->next;
	}
	chunk_t *suitable_chunk = chunk;

	// If no suitable chunk was found, allocate a new chunk using sbrk
	if (suitable_chunk == NULL) {
		void *new_memory = sbrk(adjusted_size);
		if (new_memory == (void*) -1) {
			// sbrk failed
			return NULL;
		}

		chunk_t *new_chunk = (chunk_t *) new_memory;
		new_chunk->size = adjusted_size;
		new_chunk->next = NULL;

		// Add the new chunk to the free list
		if (last_chunk == NULL) {
			free_list = new_chunk;
		} else {
			last_chunk->next = new_chunk;
		}
		last_chunk = new_chunk;

		// Update the suitable_chunk pointer
		suitable_chunk = new_chunk;
	}

	// Split the suitable chunk if it's too large
	if (suitable_chunk->size > adjusted_size) {
		size_t remaining_size  =suitable_chunk->size - adjusted_size;
		// Create a new chunk with the remaining memory
		chunk_t *new_chunk = (chunk_t *) (((char *) suitable_chunk) + adjusted_size);
		new_chunk->size = remaining_size;
		new_chunk->next = suitable_chunk->next;

		// Update the suitable chunk
		suitable_chunk->size = adjusted_size;
		suitable_chunk->next = new_chunk;
	}

	// Remove the suitable chunk from the free list
	if (prev_chunk == NULL) {
		free_list = suitable_chunk->next;
	} else {
		prev_chunk->next = suitable_chunk->next;
	}

	// Update the last chunk pointer
	last_chunk = suitable_chunk;

	// Unlock after accessing shared resources
	pthread_mutex_unlock(&mutex);

	// Return a pointer to the user memory (skip the metadata)
	return (void *) ((char *)suitable_chunk + sizeof(chunk_t));

}

void dealloc(void *ptr) {
	// Lock before accessing shared resources
	pthread_mutex_lock(&mutex);

    // If the pointer is NULL, return immediately
    if (ptr == NULL) {
        return;
    }

    // Get the pointer to the chunk metadata
    chunk_t *chunk = (chunk_t *)((char *)ptr - sizeof(chunk_t));

    // Insert the freed chunk at the beginning of the free list
    chunk->next = free_list;
    free_list = chunk;

    // Update the last_chunk pointer if necessary
    if (last_chunk == NULL || chunk > last_chunk) {
        last_chunk = chunk;
    }

	// Unlock after accessing shared resources
	pthread_mutex_unlock(&mutex);
}