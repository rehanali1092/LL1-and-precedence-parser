# Parser Implementations in C++

This repository contains two parser programs written in C++ for compiler construction practice.

## Files

### `task1.c++`
Implements an LL(1) predictive parser for the grammar:

- `E → T E'`
- `E' → + T E' | ε`
- `T → F T'`
- `T' → * F T' | ε`
- `F → ( E ) | id`

Features:
- Manual LL(1) parsing table built in the program
- Stack-based parsing implementation
- Tokenizes `id`, `+`, `*`, `(`, `)`, and end-of-input marker `$`
- Displays stack contents, remaining input, and the applied production or match action
- Prints `String Accepted` for valid input and `String Rejected` otherwise

### `task2.c++`
Implements an operator precedence parser for arithmetic expressions using:
- `+`, `-`, `*`, `/`
- `(`, `)`
- `id`

Features:
- Operator precedence table with `<`, `>`, `=` relations
- Stack-based shift/reduce parsing
- Displays stack contents, input buffer, and parsing action (`Shift`, `Reduce`, `Accept`, `Reject`)
- Accepts valid expressions like `id+id*id` and `(id+id)*id`
- Rejects invalid expressions

## Build and Run

Use `g++` to compile each file:

```bash
g++ task1.c++ -o task1.exe
g++ task2.c++ -o task2.exe
```

Run the programs:

```bash
./task1.exe
./task2.exe
```

On Windows PowerShell:

```powershell
.\task1.exe
.\task2.exe
```

## Example Input

- `id+id*id`
- `(id+id)*id`

## Notes

- Both programs treat `id` as a two-character token.
- `task1.c++` is for LL(1) predictive parsing.
- `task2.c++` is for operator precedence parsing.
- The parsers display parsing steps and final acceptance or rejection.
