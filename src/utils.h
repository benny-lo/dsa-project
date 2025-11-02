#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"

/*
 * Converts a character to its corresponding index in the alphabet [0, 64).
 * Parameters:
 * - char c: Character to convert
 * Returns: Index of the character in the alphabet
 */
static inline size_t char_index(char c) {
  int char_index;

  if (c == '-') {
    char_index = 0;
  } else if (c - '0' >= 0 && c - '0' < 10) {
    char_index = c - '0' + 1;
  } else if (c - 'A' >= 0 && c - 'A' < 26) {
    char_index = c - 'A' + 11;
  } else if (c == '_') {
    char_index = 37;
  } else if (c - 'a' >= 0 && c - 'a' < 26) {
    char_index = c - 'a' + 38;
  } else {
    char_index = ALPHABET_SIZE;
    fprintf(stderr, "error: failed conversion of %c\n", c);
  }

  return char_index;
}

/*
 * Copies a substring from source to destination.
 * Parameters:
 * - char* dest: Destination string where the substring will be copied
 * - const char* source: Source string from which the substring will be copied
 * - size_t a: Starting index in the source string
 * - size_t b: Ending index in the source string
 */
static inline void substring_copy(char *dest, char const *source, size_t a, size_t b) {
  strncpy(dest, source + a, b - a);
  dest[b - a] = '\0';
}


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