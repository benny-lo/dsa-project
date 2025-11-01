#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

#include <stdlib.h>

typedef struct {
    void *start_ptr;
    void *current_ptr;
    void *end_ptr;
} memory_arena_t;

typedef struct memory_arenas_list_node_t {
    memory_arena_t arena;
    struct memory_arenas_list_node_t *next;
} memory_arenas_list_node_t;

typedef struct {
    memory_arenas_list_node_t *arenas_list;
    size_t arena_size;
} memory_arenas_manager_t;

/* Initializes a memory arenas manager with a specified arena size.
 * Parameters:
 * - size_t arena_size: Size of each memory arena to be managed.
 */
memory_arenas_manager_t *init_memory_arenas_manager(size_t arena_size);

/* Allocates a block of memory of the specified size from the memory arenas manager.
 * If necessary, it creates new arenas to satisfy the allocation request.
 */
void *alloc(memory_arenas_manager_t *manager, size_t size);

/* Deallocates all memory arenas managed by the memory arenas manager.
 * Parameters:
 * - memory_arenas_manager_t* manager: Pointer to the memory arenas manager to deallocate.
 */
void dealloc_memory_arenas_manager(memory_arenas_manager_t *manager);


#endif // MEMORY_ARENA_H