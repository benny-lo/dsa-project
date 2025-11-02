# Algorithms and Data Structures (2021/22) - Final Project
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

- **`PRINT_FILTERED`**, which instructs the program to output the number and list of strings currently in the dictionary that a perfectly rational player could still guess, given the sequence of previous guesses and their corresponding feedback.

See the full project description in [Project description (Italian)](ProvaFinale2022.pdf).
