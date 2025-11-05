# Data Structures & Algorithms (2021/22) - Final Project
## WordChecker
The goal of this project is to implement **WordChecker** (essentially Wordle) efficiently.

The input begins with an integer `k`, representing the length of all subsequent strings. After that, an arbitrary number of strings is provided — these form the initial dictionary.

For each game (initiated with the `NEW_GAME` command), two inputs are given: a dictionary string `ref` and an integer `n`. These represent, respectively, the target string to be guessed and the number of guesses allowed.

The game continues until it ends with either `ok` (the `ref` string has been correctly guessed) or `ko` (the `ref` string was not guessed within `n` attempts). During an active game, the following types of input can be provided:

- A **guess string** of length `k`.  
  - If the string does not exist in the dictionary, the program outputs `not_exists`.  
  - Otherwise, it returns Wordle-style feedback indicating which characters are:  
    - correctly guessed and in the right position,  
    - present in the word but in a different position, or  
    - completely incorrect (see `gen_constraint` in [`src/help_constraints.h`](src/help_constraints.h)).

- **`INSERT_START`**, followed by any number of strings of length `k`, and terminated by **`INSERT_END`**.  
  These strings are added to the dictionary and must also be considered in the ongoing game.

- **`PRINT_FILTERED`**, which instructs the program to output the number and sorted list of strings currently in the dictionary that a perfectly rational player could still guess, given the sequence of previous guesses and their corresponding feedback.

See the full project description in [Project description (Italian)](ProvaFinale2022.pdf).

---

# Implementation
## Dictionary Data Structure
Here, `k` denotes the length of the strings involved, and `n` represents the maximum number of strings that may be present in the dictionary during a given game.

The main design decision concerns the choice of data structure used to store dictionary strings.  
Several options exist such as hash sets or red-black trees, but I chose to use **tries**.

The main advantage of tries is that they provide **search and insertion in `O(k)` time**, which is constant with respect to `n`.  
In contrast, red-black trees have `O(k log n)` complexity, and hash sets provide `O(k)` amortized time. Moreover, tries maintain strings in lexicographical order (like red-black trees but unlike hash sets).

The primary drawback of tries is their **high memory usage**. While their space complexity is theoretically `O(kn)` — comparable to that of red-black trees and hash sets — the constant factor can be large in naive implementations.  
The main goal of this section is therefore to explain the optimizations used to reduce trie memory consumption.

---

### Naive Implementation
A straightforward trie implementation might look like this:
```c
typedef struct trie_t {
  struct trie_t *children[64];
} trie_t;
```

(We do not need a terminal flag since all strings have the same length `k` and we store only complete strings.)

However, this design is highly inefficient:  
Each node consumes `8 × 64 = 512` bytes, and we need exactly `k + 1` nodes for each string — far too much memory.

---

### Radix Tries
The first improvement is to use **radix tries** instead of standard tries.  
The idea is that trie nodes with a single child can be merged, compressing multiple edges into one:

```c
typedef struct naive_rax_t {
  struct trie_t *children[64];
  char *substr;
} naive_rax_t;
```

This optimization reduces the number of nodes required but increases the size of each node (`8 × 64 + 8 = 520` bytes) and complicates insertion and search logic — though their asymptotic complexity remains unchanged.

---

### Optimized Radix Trie
To drastically reduce node size, we change the structure so that each node only knows about its **first child** and **next sibling**:

```c
typedef struct opt_rax_t {
  struct opt_rax_t *child;
  struct opt_rax_t *sibling;
  char *substr;
} opt_rax_t;
```

Now, each node is only `8 × 3 = 24` bytes — a major improvement.  
Although this design slightly complicates insert and search operations, their time complexity remains the same.

---

### Final Optimization
The final refinement embeds the substring directly within the node using a **Flexible Array Member (FAM)**:

```c
typedef struct rax_t {
  struct rax_t *child;
  struct rax_t *sibling;
  char substr[];
} rax_t;
```

Replacing `char *substr` with `char substr[]` eliminates an extra pointer (saving 8 bytes), reduces memory fragmentation, and keeps substrings contiguous in memory.

In addition, instead of allocating every node with a separate `malloc`, the implementation uses a **custom memory allocator** that preallocates large chunks of memory.  
This significantly reduces system calls and fragmentation, improving both speed and memory efficiency.

Finally, the actual implementation of `rax_t` includes an additional field:
```c
size_t filter;
```
This field is used to efficiently prune parts of the radix trie that contain strings no longer consistent with the player’s guesses.  
(See the next section for details.)

---

## Filtering Out Strings
All accumulated constraints from player guesses are captured in a single `help_t` instance:

```c
typedef struct val_with_flag_t {
  size_t val;
  bool flag;
} val_with_flag_t;

typedef struct help_t {
  val_with_flag_t counters[64];
  bool can_appear[][64]; // dimensions: k x 64
} help_t;
```

Here:
- `counters[i].val` stores a lower bound on the number of occurrences of character `i`.  
  If `counters[i].flag == false`, the value is exact instead.
- `can_appear[i][j] == false` means that character `j` cannot appear at position `i` in the hidden word.

The function `update_filter` is called whenever a new feedback string is produced.  
It updates the trie by marking nodes for pruning, setting `filter` to the current game index if all strings in that subtree violate the accumulated constraints.

A key property of `help_t` is that constraints are **prefix-consistent**:  
If a prefix violates `help_t` (e.g., a character appears in a forbidden position or exceeds its allowed count), then any string sharing that prefix must also be invalid.  
This property allows for early pruning of entire subtrees, greatly reducing runtime.

---

# Making and Running the Project
The project uses **CMake** and has **no external dependencies**.  
To build it, run:

```bash
# In the root folder of the repository
mkdir build
cd build
cmake ..
make
```

This creates two build targets: `debug` and `release`, with their usual meanings.

The repository also includes a test suite. To run the tests:

```bash
chmod +x run_tests.sh
./run_tests.sh release
```
