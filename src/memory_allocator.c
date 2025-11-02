#include <stdlib.h>

#include "memory_allocator.h"

typedef struct memory_block_t {
  void *start_ptr;
  void *current_ptr;
  void *end_ptr;
} memory_block_t;

typedef struct memory_block_list_node_t {
  memory_block_t block;
  struct memory_block_list_node_t *next;
} memory_block_list_node_t;

typedef struct memory_allocator_t {
  memory_block_list_node_t *blocks_list;
  size_t block_size;
} memory_allocator_t;

static memory_block_list_node_t *allocate_block_list_node(size_t block_size);
static void deallocate_block_list_node(memory_block_list_node_t *node);

memory_allocator_t *init_memory_allocator(size_t block_size) {
  memory_allocator_t *allocator =
      (memory_allocator_t *)malloc(sizeof(memory_allocator_t));
  allocator->block_size = block_size;
  allocator->blocks_list = allocate_block_list_node(block_size);
  return allocator;
}

void *allocate(memory_allocator_t *allocator, size_t size) {
  if (size > allocator->block_size) {
    allocator->block_size = size;
    return allocate(allocator, size);
  }

  if ((char *)allocator->blocks_list->block.current_ptr + size <=
      (char *)allocator->blocks_list->block.end_ptr) {
    void *ptr = allocator->blocks_list->block.current_ptr;
    allocator->blocks_list->block.current_ptr =
        (char *)allocator->blocks_list->block.current_ptr + size;
    return ptr;
  }

  memory_block_list_node_t *new_blocks_list_node =
      allocate_block_list_node(allocator->block_size);
  new_blocks_list_node->next = allocator->blocks_list;
  allocator->blocks_list = new_blocks_list_node;
  return allocate(allocator, size);
}

void deallocate(memory_allocator_t *allocator) {
  memory_block_list_node_t *current = allocator->blocks_list;
  memory_block_list_node_t *next;

  while (current != NULL) {
    next = current->next;
    deallocate_block_list_node(current);
    current = next;
  }

  free(allocator);
}

memory_block_list_node_t *allocate_block_list_node(size_t block_size) {
  memory_block_list_node_t *block_list_node =
      (memory_block_list_node_t *)malloc(sizeof(memory_block_list_node_t));
  block_list_node->block.start_ptr = malloc(block_size);
  block_list_node->block.current_ptr = block_list_node->block.start_ptr;
  block_list_node->block.end_ptr =
      (char *)block_list_node->block.start_ptr + block_size;
  block_list_node->next = NULL;
  return block_list_node;
}

void deallocate_block_list_node(memory_block_list_node_t *node) {
  free(node->block.start_ptr);
  free(node);
}
