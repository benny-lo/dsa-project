#ifndef HELP_CONSTRAINTS_H
#define HELP_CONSTRAINTS_H

#include <stdlib.h>

#include "constants.h"
#include "rax.h"

/*
 * Actual constraint bookkeeping:
 * for every position, the character forced in that position ('+');
 * for every character, lower bound on the number of occurences in the string
 * ('+' and '|'); for every character, the set of positions where it cannot
 * appear ('|' and '/').
 */

/*
 * Structure to store constraints and filtering information for word matching.
 *
 * This structure maintains information about character occurrences and
 * constraints used to filter valid words in the dictionary.
 *
 * occur:   Array tracking required minimum occurrences for each character
 * option:  Array indicating if a character must match exact occurrence count
 * forced:  Array storing required characters at specific positions
 * appear:  Matrix storing allowed characters at each position
 */
typedef struct help_t {
  size_t occur[ALPHABET_SIZE];
  bool option[ALPHABET_SIZE];
  char *forced;
  bool *appear;
} help_t;

/*
 * Allocates a new help_t structure for managing constraints.
 * Parameters:
 * - size_t k: Size of the string for which constraints are being managed
 */
help_t *help_alloc(size_t k);

/*
 * Resets the help_t structure to its initial state.
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to reset
 * - size_t k: Size of the string for which constraints are being managed
 */
void help_reset(help_t *info, size_t k);

/*
 * Deallocates a help_t structure and its associated memory.
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to deallocate
 */
void help_dealloc(help_t *info);

/*
 * Updates the help_t struct  based on a guess and its
 * corresponding constraint string.
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to update
 * - char const *guess: The guessed string
 * - char *constraint: The constraint string corresponding to the guess
 */
void help_update(help_t *info, char const *guess, char const *constraint);

/*
 * Checks if a given string is compatible with the constraints stored in the
 * help_t structure. 
 * Parameters:
 * - char const *str: String to check for compatibility
 * - help_t const *info: Pointer to the help_t structure containing constraints
 * Returns: true if the string is compatible, false otherwise
 */
bool compatible(char const *str, help_t const *info);

/*
 * Updates the filter of a radix tree node based on the current constraints.
 * Parameters:
 * - rax_t *root: Pointer to the root node of the radix tree
 * - help_t *info: Pointer to the help_t structure containing constraints
 * - int game: Game index for filtering
 * Returns: size of the filtered dictionary after the update
 */
size_t update_filter(rax_t *root, help_t *info, size_t game);

#endif // HELP_CONSTRAINTS_H