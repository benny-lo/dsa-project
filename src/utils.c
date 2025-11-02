#include "constants.h"
#include "utils.h"

void gen_constraint(char const *ref, char const *guess, char *constraint,
                    size_t k) {
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
    if (constraint[i] != NONE)
      continue;

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