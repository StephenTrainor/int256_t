#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "int256_t.h"

static int256_t** bigInts = NULL;
static size_t allocations = 0;

void set_n(int256_t *restrict bigInt, int64_t n); // Prevents implicit declaration warning

int256_t* decl_int256_t(void) {
    if (!allocations) {
        atexit(free_int256_t); // I'm not that fancy
    }
    
    int256_t* temp = malloc(sizeof(int256_t));

    if (!temp) {
        printf("NULL pointer.");
        return NULL;
    }

    int256_t** tmp = realloc(bigInts, sizeof(int256_t) * (allocations + 1)); // Grow tmp to hold another int256_t

    if (!tmp) {
        free(temp);
        return NULL;
    }

    bigInts = tmp;
    bigInts[allocations] = temp; // store int256_t to be freed later
    allocations++;

    clear(temp);

    return temp;
}

int256_t* init_n(const int64_t n) {
    int256_t* temp = init_int256_t(to_int256_t(n));

    if (!temp) {
        return NULL;
    }

    return temp;
}

int256_t* init_int256_t(const int256_t *restrict bigInt) {
    int256_t* temp = decl_int256_t();

    if (!temp) {
        return NULL;
    }

    set_int256_t(temp, bigInt);

    return temp;
}

int256_t* to_int256_t(const int64_t n) {
    int256_t* temp = decl_int256_t();

    if (!temp) {
        return NULL;
    }

    set_n(temp, n);

    return temp;
}

int256_t* multiply_n(const int256_t *restrict bigInt, uint32_t n) {
    int256_t* temp = decl_int256_t();
    
    if (!temp) {
        return init_int256_t(bigInt);
    }
    
    set_int256_t(temp, bigInt); // Copy value into temp int256_t in case of an overflow

    if (n < 0) { // handle negatives
        n *= -1;
        temp->positive = !bigInt->positive;
    }

    int i = 0;
    int carry = 0;

    for (; i < DIGITS; i++) {
        temp->v[i] *= n;
        temp->v[i] += carry;
        carry = temp->v[i] / 10;
        temp->v[i] %= 10;
    }

    if (carry || overflow(temp)) { // carry is non-zero in the case of an overflow
        printf("Overflow encountered when multiplying.");
        return init_int256_t(bigInt);
    }

    return temp;
}

int256_t* add_int256_t(const int256_t *restrict bigInt, const int256_t *restrict a) {
    if (bigInt->positive ^ a->positive) { // Wow, hackerman x2
        printf("add_int256_t() only accepts int256_t's that are both positive or both negative.\n"); // This code can't handle subtractions
        return init_int256_t(bigInt);
    }

    int256_t* temp = decl_int256_t();

    if (!temp) {
        printf("NULL pointer.");
        return init_int256_t(bigInt);
    }
    
    set_int256_t(temp, bigInt);

    int i = 0;
    int carry = 0;

    for (; i < DIGITS; i++) {
        temp->v[i] += a->v[i];
        temp->v[i] += carry;
        carry = temp->v[i] / 10;
        temp->v[i] %= 10;
    }

    if (carry || overflow(temp)) { // carry is non-zero in the case of an overflow
        printf("Overflow encountered when adding.");
        return init_int256_t(bigInt);
    }

    return temp;
}

void clear(int256_t *restrict bigInt) {
    // memset(bigInt->v, 0, sizeof(uint64_t) * DIGITS - 7); // Same as below
    memset(bigInt->v, 0, sizeof(uint64_t) * DIGITS);
    bigInt->positive = true;
}

void set_n(int256_t *restrict bigInt, int64_t n) {
    clear(bigInt);

    bigInt->positive = true;
    if (n < 0) { // handle negatives
        n *= -1;
        bigInt->positive = false;
    }

    int i = 0;
    int64_t divisor = 1;

    for (; i < 20; i++, divisor *= 10) {
        bigInt->v[i] = (n / divisor) % 10; // Get each digit of n individually
    }
}

void set_int256_t(int256_t *restrict bigInt, const int256_t *restrict a) {
    clear(bigInt);

    bigInt->positive = a->positive;

    int i = 0;

    for (; i < DIGITS; i++) {
        bigInt->v[i] = a->v[i];
    }
}

void display_int256_t(const int256_t *restrict bigInt) {
    int i = DIGITS - 1;

    while (!bigInt->v[i]) { // Skip over the leading zeros that may exist
        i--;
    }

    if (i < 0) {
        printf("0"); // If we skipped over every zero, then it's zero
        return;
    }
    
    if (!bigInt->positive) {
        printf("-");
    }

    for (; i >= 0; i--) {
        printf("%lu", bigInt->v[i]);
    }
}

bool overflow(const int256_t *restrict bigInt) {
    if (!bigInt) {
        printf("NULL int256_t encountered.\n");
        return true;
    }

    if (!bigInt->v[DIGITS - 1]) { // Lazy check for if we are close to 2^256 - 1
        return false;
    }

    int8_t MAX_SEQ[] = {
        5, 3, 9, 9, 3, 6, 9, 2, 1, 3, 1, 9, 7, 
        0, 0, 4, 8, 5, 7, 5, 4, 9, 3, 0, 4, 6, 
        5, 0, 4, 6, 5, 6, 6, 4, 8, 9, 9, 6, 2, 
        3, 5, 8, 7, 0, 9, 7, 8, 6, 8, 0, 0, 5, 
        8, 9, 0, 7, 5, 3, 2, 4, 5, 9, 1, 6, 1, 
        3, 7, 3, 2, 9, 8, 0, 2, 9, 7, 5, 1, 1
    }; // The digits of 2^256 - 1 in reverse, see le.py

    if (!bigInt->positive) {
        MAX_SEQ[0]++;
    }

    int i = DIGITS - 1;

    for (; i; i--) {
        if (bigInt->v[i] > MAX_SEQ[i]) {
            return true;
        }
    }

    return false;
}

/*
Idea from:

https://github.com/cs50/libcs50/blob/main/src/cs50.c#:~:text=static%20void%20teardown(void)
https://stackoverflow.com/questions/24930162/exists-a-way-to-free-memory-in-atexit-or-similar-without-using-global-variables
*/
void free_int256_t(void) {
    if (bigInts) {
        for (size_t i = 0; i < allocations; i++) {
            free(bigInts[i]); // Free every int256_t that was previously stored
        }
        free(bigInts); 
    }
}
