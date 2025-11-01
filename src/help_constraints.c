#include <stdlib.h>
#include <stdbool.h>

#include "help_constraints.h"
#include "constants.h"
#include "utils.h"

static void reset(char const *, size_t *, size_t);
static size_t update_filter_aux(rax_t *root, size_t *str_occur, size_t curr_idx, help_t *info,
                     size_t game);

help_t *help_alloc(size_t k) {
  help_t *info = (help_t *)malloc(sizeof(help_t));
  info->forced = (char *)malloc((k + 1) * sizeof(char));
  info->appear = (bool *)malloc(k * ALPHABET_SIZE * sizeof(bool));

  return info;
}

void help_reset(help_t *info, size_t k) {
  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    info->option[i] = false;
    info->occur[i] = 0;
  }

  for (size_t i = 0; i < k; i++) {
    info->forced[i] = NONE;
    for (int j = 0; j < ALPHABET_SIZE; j++) {
      info->appear[i * ALPHABET_SIZE + j] = true;
    }
  }
  info->forced[k] = '\0';
}

void help_dealloc(help_t *info) {
  if (info == NULL)
    return;
  if (info->forced != NULL)
    free(info->forced);
  if (info->appear != NULL)
    free(info->appear);

  free(info);
}

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
    size_t index = char_index(guess[i]);

    if (constraint[i] == '+') {
      running_notslash[index]++;
      info->forced[i] = guess[i];
      if (!info->option[index] && running_notslash[index] > info->occur[index]) {
        info->occur[index] = running_notslash[index];
      }
    } else if (constraint[i] == '|') {
      info->appear[i * ALPHABET_SIZE + index] = false;
      running_notslash[index]++;
      if (!info->option[index] && running_notslash[index] > info->occur[index]) {
        info->occur[index] = running_notslash[index];
      }
    } else if (constraint[i] == '/') {
      info->appear[i * ALPHABET_SIZE + index] = false;
      info->occur[index] = total_notslash[index];
      info->option[index] = true;
    }
  }
}

bool compatible(char const *str, help_t const *info) {
  size_t str_occur[ALPHABET_SIZE] = {0};

  for (size_t i = 0; str[i] != '\0'; i++) {
    str_occur[char_index(str[i])]++;
    if (info->forced[i] != NONE) {
      if (info->forced[i] != str[i])
        return false;
    } else {
      if (!info->appear[i * ALPHABET_SIZE + char_index(str[i])])
        return false;
    }
  }

  for (size_t i = 0; i < ALPHABET_SIZE; i++) {
    if (info->option[i]) {
      if (info->occur[i] != str_occur[i])
        return false;
    } else {
      if (info->occur[i] > str_occur[i])
        return false;
    }
  }

  return true;
}

size_t update_filter(rax_t *root, help_t *info, size_t game) {
  size_t str_occur[ALPHABET_SIZE] = {0};
  return update_filter_aux(root, str_occur, 0, info, game);
}

size_t update_filter_aux(rax_t *root, size_t *str_occur, size_t curr_idx, help_t *info,
                     size_t game) {
  if (root->filter == game)
    return 0;

  size_t piece_idx, ans = 0;
  rax_t *tmp;

  for (piece_idx = 0; root->piece[piece_idx] != '\0'; piece_idx++) {
    // Register occurrence of the character at position piece_idx in root->piece
    str_occur[char_index(root->piece[piece_idx])]++;

    if (info->forced[curr_idx + piece_idx] != NONE) {
      if (info->forced[curr_idx + piece_idx] != root->piece[piece_idx]) {
        root->filter = game;
        reset(root->piece, str_occur, piece_idx);
        return 0;
      }
    } else {
      if (!info->appear[(curr_idx + piece_idx) * ALPHABET_SIZE +
                        char_index(root->piece[piece_idx])]) {
        root->filter = game;
        reset(root->piece, str_occur, piece_idx);
        return 0;
      }
    }

    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (info->option[i] && str_occur[i] > info->occur[i]) {
        root->filter = game;
        reset(root->piece, str_occur, piece_idx);
        return 0;
      }
    }
  }

  tmp = root->child;
  if (tmp == NULL) {
    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
      if (info->option[i]) {
        if (str_occur[i] != info->occur[i]) {
          root->filter = game;
          reset(root->piece, str_occur, piece_idx);
          return 0;
        }
      } else {
        if (str_occur[i] < info->occur[i]) {
          root->filter = game;
          reset(root->piece, str_occur, piece_idx);
          return 0;
        }
      }
    }

    root->filter = 0;
    reset(root->piece, str_occur, piece_idx);
    return 1;
  } else {
    while (tmp != NULL) {
      ans += update_filter_aux(tmp, str_occur, curr_idx + piece_idx, info, game);
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
