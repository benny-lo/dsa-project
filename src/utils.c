#include "constants.h"
#include "utils.h"

static const char PERFECT_MATCH = '+';
static const char PARTIAL_MATCH = '|';
static const char NO_MATCH = '/';

void gen_constraint(char const *ref, char const *guess, char *feedback,
                    size_t k) {
  size_t ref_occur_not_perfect_match[ALPHABET_SIZE] = {0};

  // first loop: finds perfect matches and puts a '+' in feedback and
  // counts occurrences of non-perfect-matched characters in ref
  for (size_t i = 0; i < k; i++) {
    ref_occur_not_perfect_match[char_index(ref[i])]++;

    if (ref[i] == guess[i]) {
      feedback[i] = PERFECT_MATCH;
      ref_occur_not_perfect_match[char_index(ref[i])]--;
    } else {
      feedback[i] = '\0'; // not the actual value, just a filler != PERFECT_MATCH
    }
  }

  // second loop: takes care of filling feedback with '|' and '/' for 
  // mismatched characters between guess and ref
  for (size_t i = 0; i < k; i++) {
    if (feedback[i] == PERFECT_MATCH)
      continue;

    size_t index = char_index(guess[i]);
    if (ref_occur_not_perfect_match[index] != 0) {
      feedback[i] = PARTIAL_MATCH;
      ref_occur_not_perfect_match[index]--;
    } else {
      feedback[i] = NO_MATCH;
    }
  }

  feedback[k] = '\0';
}