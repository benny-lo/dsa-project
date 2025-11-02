#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

/*
 * Converts a character to its corresponding index in the alphabet [0, 64).
 * Parameters:
 * - char c: Character to convert
 * Returns: Integer index of the character in the alphabet
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
static inline void substring_copy(char *dest, char const *source, size_t a,
                                  size_t b) {
  strncpy(dest, source + a, b - a);
  dest[b - a] = '\0';
}

/*
 * Generates feedback string based on the reference (hidden to the user)
 * and guess strings. The rules for generating the feedback are as follows:
 * - feedback[i] = '+' if guess[i] == ref[i]
 * - feedback[i] = '/' if guess[i] does not appear in ref at all
 * - feedback[i] = '|' if guess[i] appears in ref but guess[i] != ref[i];
 *   however, if the number of occurences of guess[i] in ref is n, the number
 *   of occurences of guess[i] in guess that match perfectly with ref is c and
 *   there are at least n - c occurences of guess[i] in guess before position i
 *   and not matching perfectly with ref, then feedback[i] = '/'.
 * Parameters:
 * - char const *ref: Reference string
 * - char const *guess: Guess string
 * - char *feedback: Output buffer (of size at least k + 1) to store the
 * generated feedback string
 * - size_t k: Length of the strings
 */
void gen_constraint(char const *ref, char const *guess, char *feedback,
                    size_t k);

#endif
