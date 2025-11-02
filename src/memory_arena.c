#include <stdlib.h>

#include "memory_arena.h"

typedef struct memory_arena_t {
  void *start_ptr;
  void *current_ptr;
  void *end_ptr;
} memory_arena_t;

typedef struct memory_arenas_list_node_t {
  memory_arena_t arena;
  struct memory_arenas_list_node_t *next;
} memory_arenas_list_node_t;

typedef struct memory_arenas_manager_t {
  memory_arenas_list_node_t *arenas_list;
  size_t arena_size;
} memory_arenas_manager_t;

static memory_arenas_list_node_t *allocate_arenas_list_node(size_t arena_size);
static void deallocate_arenas_list_node(memory_arenas_list_node_t *node);

memory_arenas_manager_t *init_memory_arenas_manager(size_t arena_size) {
  memory_arenas_manager_t *manager =
      (memory_arenas_manager_t *)malloc(sizeof(memory_arenas_manager_t));
  manager->arena_size = arena_size;
  manager->arenas_list = allocate_arenas_list_node(arena_size);
  return manager;
}

void *alloc(memory_arenas_manager_t *manager, size_t size) {
  if (size > manager->arena_size) {
    manager->arena_size = size;
    return alloc(manager, size);
  }

  if ((char *)manager->arenas_list->arena.current_ptr + size <=
      (char *)manager->arenas_list->arena.end_ptr) {
    void *ptr = manager->arenas_list->arena.current_ptr;
    manager->arenas_list->arena.current_ptr =
        (char *)manager->arenas_list->arena.current_ptr + size;
    return ptr;
  }

  memory_arenas_list_node_t *new_arenas_list_node =
      allocate_arenas_list_node(manager->arena_size);
  new_arenas_list_node->next = manager->arenas_list;
  manager->arenas_list = new_arenas_list_node;
  return alloc(manager, size);
}

void dealloc_memory_arenas_manager(memory_arenas_manager_t *manager) {
  memory_arenas_list_node_t *current = manager->arenas_list;
  memory_arenas_list_node_t *next;

  while (current != NULL) {
    next = current->next;
    deallocate_arenas_list_node(current);
    current = next;
  }

  free(manager);
}

memory_arenas_list_node_t *allocate_arenas_list_node(size_t arena_size) {
  memory_arenas_list_node_t *arenas_list_node =
      (memory_arenas_list_node_t *)malloc(sizeof(memory_arenas_list_node_t));
  arenas_list_node->arena.start_ptr = malloc(arena_size);
  arenas_list_node->arena.current_ptr = arenas_list_node->arena.start_ptr;
  arenas_list_node->arena.end_ptr =
      (char *)arenas_list_node->arena.start_ptr + arena_size;
  arenas_list_node->next = NULL;
  return arenas_list_node;
}

void deallocate_arenas_list_node(memory_arenas_list_node_t *node) {
  free(node->arena.start_ptr);
  free(node);
}
