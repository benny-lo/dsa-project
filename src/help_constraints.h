#ifndef HELP_CONSTRAINTS_H
#define HELP_CONSTRAINTS_H

#include <stdlib.h>

#include "constants.h"
#include "rax.h"

/*
 * Structure to keep track of the information about the hidden string accumulated
 * through the user guesses so far. Essentially, it represents the strings in the dictionary
 * that a perfectly rational user should not guess given the previous guesses and 
 * corresponding feedbacks.
 */
typedef struct help_t help_t;

/*
 * Allocates a new help_t structure.
 * Parameters:
 * - size_t k: Size of the string in the dictionary
 * Returns: Pointer to the newly allocated help_t structure
 */
help_t *help_alloc(size_t k);

/*
 * Resets the help_t structure to its initial state (all strings in the dictionary).
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to reset
 * - size_t k: Size of the string in the dictionary
 */
void help_reset(help_t *info, size_t k);

/*
 * Deallocates a help_t structure and its associated memory.
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to deallocate
 */
void help_dealloc(help_t *info);

/*
 * Updates the help_t struct based on a guess and its
 * corresponding feedback string.
 * Parameters:
 * - help_t *info: Pointer to the help_t structure to update
 * - char const *guess: The guessed string
 * - char const *feedback: The feedback string corresponding to the guess
 */
void help_update(help_t *info, char const *guess, char const *feedback);

/*
 * Checks if a given string is can be guessed by a perfectly rational user
 * given the information acculamated so far through guesses and correspoding feedbacks. 
 * Parameters:
 * - char const *str: String to check for compatibility (null-terminated)
 * - help_t const *info: Pointer to the help_t structure containing constraints
 * Returns: true if the string is compatible, false otherwise
 */
bool compatible(char const *str, help_t const *info);

/*
 * Counts the number of strings in the radix tree (the dictionary) that are
 * compatible with the information encapsulated by`info`. Moreover, it updates 
 * the filter fields of the nodes corresponding to branches of the radix tree 
 * that can be pruned away.
 * Parameters:
 * - rax_t *root: Pointer to the root node of the radix tree
 * - help_t *info: Pointer to the help_t structure
 * - int game: Game index
 * Returns: Number of strings in the radix tree compatible with `info`
 */
size_t update_filter(rax_t *root, help_t *info, size_t game);

#endif // HELP_CONSTRAINTS_H
