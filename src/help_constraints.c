#include <stdbool.h>
#include <stdlib.h>

#include "constants.h"
#include "help_constraints.h"
#include "utils.h"

typedef struct val_with_flag_t {
  size_t val;
  bool flag;
} val_with_flag_t;

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
 * counters[i].flag:  If true, counters[i].val is exact else it is a lower bound
 * counters[i].val:   Lower boud or exact number of occurrences of character i
 * in the goal string can_appear[i][j]:  If false, character j does not appear
 * at position i in the goal string
 */
typedef struct help_t {
  val_with_flag_t counters[ALPHABET_SIZE];
  bool can_appear[];
} help_t;

static void reset(char const *, size_t *, size_t);
static size_t update_filter_aux(rax_t *root, size_t *str_occur, size_t curr_idx,
                                help_t *info, size_t game);

help_t *help_alloc(size_t k) {
  help_t *new_info =
      (help_t *)malloc(sizeof(help_t) + k * ALPHABET_SIZE * sizeof(bool));
  return new_info;
}

void help_reset(help_t *info, size_t k) {
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    info->counters[i].val = 0;
    info->counters[i].flag = false;
  }

  for (size_t i = 0; i < k * ALPHABET_SIZE; i++) {
    info->can_appear[i] = true;
  }
}

void help_dealloc(help_t *info) { free(info); }

void help_update(help_t *info, char const *guess, char const *constraint) {
  size_t total_notslash[ALPHABET_SIZE] = {0};
  size_t running_notslash[ALPHABET_SIZE] = {0};

  // count total non-'/' occurrences per letter
  for (size_t i = 0; constraint[i] != '\0'; i++) {
    if (constraint[i] != '/') {
      total_notslash[char_index(guess[i])]++;
    }
  }

  // main loop
  for (size_t i = 0; constraint[i] != '\0'; i++) {
    size_t c_index = char_index(guess[i]);

    if (constraint[i] == '+') {
      running_notslash[c_index]++;

      for (size_t j = 0; j < ALPHABET_SIZE; j++) {
        if (j != c_index) {
          info->can_appear[i * ALPHABET_SIZE + j] = false;
        }
      }

      if (!info->counters[c_index].flag &&
          running_notslash[c_index] > info->counters[c_index].val) {
        info->counters[c_index].val = running_notslash[c_index];
      }
    } else if (constraint[i] == '|') {
      info->can_appear[i * ALPHABET_SIZE + c_index] = false;

      running_notslash[c_index]++;

      if (!info->counters[c_index].flag &&
          running_notslash[c_index] > info->counters[c_index].val) {
        info->counters[c_index].val = running_notslash[c_index];
      }
    } else if (constraint[i] == '/') {
      info->can_appear[i * ALPHABET_SIZE + c_index] = false;

      info->counters[c_index].flag = true;
      info->counters[c_index].val = total_notslash[c_index];
    }
  }
}

bool compatible(char const *str, help_t const *info) {
  size_t str_occur[ALPHABET_SIZE] = {0};

  for (size_t i = 0; str[i] != '\0'; i++) {
    size_t c_index = char_index(str[i]);
    str_occur[c_index]++;

    if (!info->can_appear[i * ALPHABET_SIZE + c_index])
      return false;
  }

  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    size_t val = info->counters[i].val;
    if (val > str_occur[i] || (info->counters[i].flag && val < str_occur[i]))
      return false;
  }

  return true;
}

size_t update_filter(rax_t *root, help_t *info, size_t game) {
  size_t str_occur[ALPHABET_SIZE] = {0};
  return update_filter_aux(root, str_occur, 0, info, game);
}

size_t update_filter_aux(rax_t *root, size_t *str_occur, size_t curr_idx,
                         help_t *info, size_t game) {
  if (root->filter == game)
    return 0;

  size_t piece_idx, ans = 0;
  rax_t *tmp;

  for (piece_idx = 0; root->piece[piece_idx] != '\0'; piece_idx++) {
    size_t c_index = char_index(root->piece[piece_idx]);
    str_occur[c_index]++;

    if (!info->can_appear[(curr_idx + piece_idx) * ALPHABET_SIZE + c_index]) {
      root->filter = game;
      reset(root->piece, str_occur, piece_idx);
      return 0;
    }

    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (info->counters[i].flag && str_occur[i] > info->counters[i].val) {
        root->filter = game;
        reset(root->piece, str_occur, piece_idx);
        return 0;
      }
    }
  }

  tmp = root->child;
  if (tmp == NULL) {
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (str_occur[i] < info->counters[i].val ||
          (info->counters[i].flag && str_occur[i] != info->counters[i].val)) {
        root->filter = game;
        reset(root->piece, str_occur, piece_idx);
        return 0;
      }
    }

    root->filter = 0;
    reset(root->piece, str_occur, piece_idx);
    return 1;
  } else {
    while (tmp != NULL) {
      ans +=
          update_filter_aux(tmp, str_occur, curr_idx + piece_idx, info, game);
      tmp = tmp->sibling;
    }

    if (ans == 0)
      root->filter = game;

    reset(root->piece, str_occur, piece_idx);
    return ans;
  }
}

static void reset(char const *str, size_t *occur, size_t up) {
  for (int i = 0; i <= up; i++) {
    if (str[i] != '\0')
      occur[char_index(str[i])]--;
  }
}
