#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

#include <stdlib.h>

typedef struct memory_allocator_t memory_allocator_t;

/*
 * Initializes a memory allocator with a specified block size.
 * Parameters:
 * - size_t block_size: Size of each memory block to be managed
 * Returns: Pointer to the initialized memory allocator
 */
memory_allocator_t *init_memory_allocator(size_t arena_size);

/*
 * Requests memory from the allocator.
 * Parameters:
 * - memory_allocator_t *allocator: Pointer to the memory allocator
 * - size_t size: Size (in bytes) of memory to allocate
 * Returns: Pointer to the allocated memory
 */
void *allocate(memory_allocator_t *allocator, size_t size);

/*
 * Deallocates all memory allocated by the allocator and frees the allocator
 * itself. Parameters:
 * - memory_allocator_t *allocator: Pointer to the memory allocator
 */
void deallocate(memory_allocator_t *allocator);

#endif // MEMORY_ARENA_H
