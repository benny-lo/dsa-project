#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

/*
 * Converts a character to its corresponding index in the alphabet [0, 64).
 * Parameters:
 * - char c: Character to convert
 * Returns: Index of the character in the alphabet
 */
size_t char_index(char c);

/*
 * Copies a substring from source to destination.
 * Parameters:
 * - char* dest: Destination string where the substring will be copied
 * - const char* source: Source string from which the substring will be copied
 * - size_t a: Starting index in the source string
 * - size_t b: Ending index in the source string
 */
void substring_copy(char *dest, char const *source, size_t a, size_t b);

/*
 * Generates constraints based on the reference and guess strings.
 * Updates the help_t structure with the new constraints.
 * Parameters:
 * - const char* ref: Reference string
 * - const char* guess: Guess string
 * - char* constraint: String to store the generated constraints
 * - help_t* info: Pointer to the help_t structure to update
 * - int k: Length of the strings
 */
void gen_constraint(char const *ref, char const *guess, char *constraint,
                    size_t k);

#endif