#ifndef INT256_H
#define INT256_H

#include <stdint.h>
#include <stdbool.h>

#define DIGITS 78

typedef struct {
    bool positive;
    uint64_t v[DIGITS]; // enough for at least 2^256-1
} int256_t;

int256_t* init_int256_t(void);
int256_t* multiply_n(const int256_t *restrict bigInt, const uint32_t n);
void clear(int256_t *restrict bigInt);
void set_n(int256_t *restrict bigInt, int64_t n);
void set_int256_t(int256_t *restrict bigInt, const int256_t *restrict a);
void add_n(int256_t *restrict bigInt, const uint64_t n);
void add_int256_t(int256_t* bigInt, const int256_t *restrict a);
void display_int256_t(const int256_t *restrict bigInt);
bool overflow(const int256_t *restrict bigInt);
void free_int256_t(void);

#endif
