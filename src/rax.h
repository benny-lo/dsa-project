#ifndef RAX_H
#define RAX_H

#include <stdbool.h>
#include <stdlib.h>

/* Radix tree node structure.
 * Members:
 * - size_t filter: Keeps track of the non-ammissible paths down the tree for
 * game with index `filter` (game indices are 1-based).
 * - rax_t* sibling: Pointer to the next sibling node.
 * - rax_t* child: Pointer to the first child node.
 * - char[] piece: String substring store in-place (null-terminated).
 */
typedef struct rax_t {
  size_t filter;
  struct rax_t *child;
  struct rax_t *sibling;
  char piece[];
} rax_t;

/* Allocates a new empty radix tree. Ownership is transferred to the caller.
 * Returns: Pointer to the root of the newly allocated radix tree.
 */
rax_t *rax_alloc();

/* Recursively deallocates an entire radix tree.
 * Parameters:
 * - rax_t* root: Root node of the tree to deallocate.
 */
void rax_dealloc(rax_t *root);

/* Searches for a string in the radix tree, ignoring the filtered out strings.
 * Parameters:
 * - rax_t* root: Root node of the tree.
 * - char* str: String to search for.
 * Returns: true if string is found, false otherwise.
 */
bool rax_search(rax_t const *root, char const *str);

/* Inserts a string in the radix tree, with a given game filter.
 * Parameters:
 * - rax_t* root: Root node of the tree.
 * - const char* str: String to insert.
 * - size_t str_size: Size of the string to insert.
 * - size_t game: Game index for filtering (zero not to filter out the inserted
 * string).
 */
void rax_insert(rax_t *root, char const *str, size_t str_size, size_t game);

/* Prints the strings stored in the radix tree.
 * Parameters:
 * - rax_t* root: Root node of the tree.
 * - char* str: Buffer to store the printed strings (must be large enough).
 * - size_t game: Game index for filtering (zero not to filter out any string in
 * the tree).
 */
void rax_print(rax_t const *root, char *str, size_t game);

/* Returns the number of strings stored in the radix tree.
 * Parameters:
 * - rax_t* root: Root node of the tree.
 * - size_t game: Game index for filtering.
 * Returns: Number of strings in the tree.
 */
size_t rax_size(rax_t const *root, size_t game);

#endif