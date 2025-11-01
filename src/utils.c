#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "utils.h"


size_t char_index(char c) {
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

void gen_constraint(char const *ref, char const *guess, char *constraint, size_t k) {
  size_t ref_occur_not_perfect_match[ALPHABET_SIZE] = {0};

  for (size_t i = 0; i < k; i++) {
    ref_occur_not_perfect_match[char_index(ref[i])]++;

    if (ref[i] == guess[i]) {
      constraint[i] = '+';
      ref_occur_not_perfect_match[char_index(ref[i])]--;
    } else {
      constraint[i] = NONE;
    }
  }

  for (size_t i = 0; i < k; i++) {
    if (constraint[i] != NONE) continue;

    size_t index = char_index(guess[i]);
    if (ref_occur_not_perfect_match[index] != 0) {
      constraint[i] = '|';
      ref_occur_not_perfect_match[index]--;
    } else {
      constraint[i] = '/';
    }
  }

  constraint[k] = '\0';
}

void substring_copy(char *dest, char const *source, size_t a, size_t b) {
  for (int i = 0; i < b - a; i++) {
    dest[i] = source[a + i];
  }
  dest[b - a] = '\0';
}
