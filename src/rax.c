#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory_arena.h"
#include "rax.h"
#include "utils.h"

static rax_t *rax_alloc_node(memory_arenas_manager_t *manager, size_t k);

static bool rax_search_aux(rax_t const *root, char const *str, size_t curr_idx);
/*
 * Searches for a node in a list of siblings whose piece starts with the given
 * character. Parameters:
 * - rax_t *child: Pointer to the first node, all of its siblings will be
 * considered
 * - char to_find: Character that node->piece[0] has to match
 * Returns: Pointer to the found node, or NULL if no such node exists
 */
static rax_t *rax_search_child(rax_t *child, char to_find);

static void rax_insert_aux(memory_arenas_manager_t *manager, rax_t *root,
                           char const *str, size_t curr_idx, size_t str_size,
                           size_t game);
static rax_t *rax_insert_child(rax_t *child, rax_t *to_ins);

static void rax_print_aux(rax_t const *root, char *str, size_t curr_idx,
                          size_t game);

rax_t *rax_alloc(memory_arenas_manager_t *manager) {
  return rax_alloc_node(manager, 0);
}

bool rax_search(rax_t const *root, char const *str) {
  return rax_search_aux(root, str, 0);
}

void rax_insert(memory_arenas_manager_t *manager, rax_t *root, char const *str,
                size_t str_size, size_t game) {
  rax_insert_aux(manager, root, str, 0, str_size, game);
}

void rax_print(rax_t const *root, char *str, size_t game) {
  rax_print_aux(root, str, 0, game);
}

size_t rax_size(rax_t const *root, size_t game) {
  if (root->filter == game)
    return 0; // node and subtree filtered out

  size_t ans = 0;
  rax_t *tmp;

  // Recurring on first child and siblings
  tmp = root->child;
  if (tmp == NULL)
    return 1;
  while (tmp != NULL) {
    ans += rax_size(tmp, game);
    tmp = tmp->sibling;
  }

  return ans;
}

rax_t *rax_alloc_node(memory_arenas_manager_t *manager, size_t k) {
  rax_t *new_node = (rax_t *)alloc(manager, sizeof(rax_t) + k + 1);

  new_node->filter = 0;
  new_node->sibling = NULL;
  new_node->child = NULL;
  new_node->piece[k] = '\0';

  return new_node;
}

bool rax_search_aux(rax_t const *root, char const *str, size_t curr_idx) {
  size_t piece_idx, new_idx;
  rax_t *good_child;

  // if str[curr_idx:curr_idx + len(root->piece)] != root->piece, return false
  for (piece_idx = 0; root->piece[piece_idx] != '\0'; piece_idx++) {
    if (root->piece[piece_idx] != str[curr_idx + piece_idx])
      return false;
  }

  // if the string is fully matched, return true
  new_idx = curr_idx + piece_idx;
  if (str[new_idx] == '\0')
    return true;

  // the string is only partially matched, so we need to search for the next
  // node to continue within the linked list of children (following the sibling
  // pointers)
  good_child = rax_search_child(root->child, str[new_idx]);
  if (good_child == NULL)
    return false;
  return rax_search_aux(good_child, str, new_idx);
}

rax_t *rax_search_child(rax_t *child, char to_find) {
  while (child != NULL) {
    if (child->piece[0] == to_find)
      return child;

    child = child->sibling;
  }

  // no node found
  return NULL;
}

void rax_insert_aux(memory_arenas_manager_t *manager, rax_t *root,
                    char const *str, size_t curr_idx, size_t str_size,
                    size_t game) {
  size_t piece_idx, new_idx, old_filter;
  rax_t *new_node, *new_node_son, *old_child, *child_find;

  old_filter = root->filter;
  if (game == 0)
    root->filter = 0;

  size_t piece_size = strlen(root->piece);

  for (piece_idx = 0; root->piece[piece_idx] != '\0'; piece_idx++) {
    if (root->piece[piece_idx] != str[curr_idx + piece_idx]) {
      // allocating the new nodes
      new_node = rax_alloc_node(manager, str_size - piece_idx - curr_idx);
      new_node_son = rax_alloc_node(manager, piece_size - piece_idx);

      // copying the correct substrings into the nodes
      substring_copy(new_node->piece, str, piece_idx + curr_idx, str_size);
      substring_copy(new_node_son->piece, root->piece, piece_idx, piece_size);
      root->piece[piece_idx] = '\0';

      // saving the children of root
      old_child = root->child;
      root->child = NULL;

      // set the filters
      new_node->filter = game;
      new_node_son->filter = old_filter;

      // setting the children of root
      root->child = rax_insert_child(root->child, new_node);
      root->child = rax_insert_child(root->child, new_node_son);
      new_node_son->child = old_child;

      return;
    }
  }

  new_idx = curr_idx + piece_size;

  if (new_idx == str_size)
    return; // the string is already present in the tree

  child_find = rax_search_child(root->child, str[new_idx]);
  if (child_find == NULL) {
    // create the new node and fill it with its substring
    new_node = rax_alloc_node(manager, str_size - new_idx);
    substring_copy(new_node->piece, str, new_idx, str_size);
    new_node->filter = game;

    // insert the new node as a child of root
    root->child = rax_insert_child(root->child, new_node);
    return;
  }

  rax_insert_aux(manager, child_find, str, new_idx, str_size, game);
}

rax_t *rax_insert_child(rax_t *child, rax_t *to_ins) {
  if (child == NULL)
    return to_ins;
  char new_val = to_ins->piece[0];
  rax_t *tmp = child;

  if (new_val > child->piece[0]) {
    while (tmp->sibling != NULL && new_val > tmp->sibling->piece[0]) {
      tmp = tmp->sibling;
    }

    to_ins->sibling = tmp->sibling;
    tmp->sibling = to_ins;
    return child;
  } else {
    to_ins->sibling = child;
    return to_ins;
  }
}

void rax_print_aux(rax_t const *root, char *str, size_t curr_idx, size_t game) {
  if (root->filter == game)
    return;

  size_t piece_idx, new_idx;
  rax_t *tmp;
  for (piece_idx = 0; root->piece[piece_idx] != '\0'; piece_idx++) {
    str[curr_idx + piece_idx] = root->piece[piece_idx];
  }

  new_idx = piece_idx + curr_idx;
  tmp = root->child;

  if (tmp == NULL) {
    str[new_idx] = '\0';
    printf("%s\n", str);
    return;
  }

  while (tmp != NULL) {
    rax_print_aux(tmp, str, new_idx, game);
    tmp = tmp->sibling;
  }
}
