# int256_t
An experimental implementation of bigIntegers with C

### Usage
Simply include the header file via `#include "int256_t.h"` or `#include "path/to/int256_t.h"`

Additionally, have `int256_t.c` in the same directory as the header file, or adjust the includes accordingly

### Memory
To avoid double frees, do not try to free any dynamically allocated `int256_t`s. `free_int256_t()` frees up the memory upon exit of the program.

### Limits
When `DIGITS` is 78, int256_t supports at least -2^256 to 2^256 - 1

`DIGITS` can simply be increased to meet your needs
