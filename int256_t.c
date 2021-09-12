#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "int256_t.h"

static int256_t** bigInts = NULL;
static size_t allocations = 0;

// static char* itoa_c(uint32_t val, int base) { // From https://www.strudel.org.uk/itoa/
// 	static char buf[32] = {0};
// 	int i = 30;
	
// 	for(; val && i ; --i, val /= base)
	
// 		buf[i] = "0123456789abcdef"[val % base];
	
// 	return &buf[i+1];
// }

int256_t* init_int256_t(void) {
    int256_t* temp = malloc(sizeof(int256_t));

    if (!temp) {
        printf("NULL pointer.");
        return NULL;
    }

    int256_t** tmp = realloc(bigInts, sizeof(int256_t) * (allocations + 1));

    if (!tmp) {
        free(temp);
        return NULL;
    }

    bigInts = tmp;

    bigInts[allocations] = temp;
    allocations++;

    temp->positive = true;
    // memset(bigInt->v, 0, DIGITS);

    for (int i = 0; i < DIGITS; i++) {
        temp->v[i] = 0x0;
    }

    return temp;
}

int256_t* multiply_n(const int256_t *restrict bigInt, uint32_t n) {
    int256_t* temp = init_int256_t();

    if (!temp) {
        printf("NULL pointer.");
        return NULL;
    }

    if (n < 0) {
        n *= -1;
        temp->positive = !bigInt->positive;
    }

    int i = 0;
    
    for (; i < DIGITS; i++) {
        temp->v[i] = bigInt->v[i];
    }

    int carry = 0;

    for (i = 0; i < DIGITS; i++) {
        temp->v[i] *= n;
        temp->v[i] += carry;
        carry = temp->v[i] / 10;
        temp->v[i] %= 10;
    }

    if (carry || overflow(temp)) {
        printf("Overflow encountered when multiplying.");
        free(temp);
        return NULL;
    }

    return temp;
}

void clear(int256_t *restrict bigInt) {
    memset(bigInt->v, 0, DIGITS);
}

void set_n(int256_t *restrict bigInt, int64_t n) {
    clear(bigInt);

    bigInt->positive = true;
    if (n < 0) {
        n *= -1;
        bigInt->positive = false;
    }

    int i = 0;
    int64_t divisor = 1;

    for (; i < 20; i++, divisor *= 10) {
        bigInt->v[i] = (n / divisor) % 10;
    }
}

void set_int256_t(int256_t *restrict bigInt, const int256_t *restrict a) {
    clear(bigInt);

    bigInt->positive = true;
    if (!a->positive) {
        bigInt->positive = false;
    }

    int i = 0;

    for (; i < DIGITS; i++) {
        bigInt->v[i] = a->v[i];
    }
}

void add_n(int256_t *restrict bigInt, uint64_t n) {
    if (n <= 0) {
        printf("add_n() only accepts positive integers (uint64_t).\n");
        return;
    }

    int256_t* temp = malloc(sizeof(int256_t));

    if (!temp) {
        printf("NULL pointer.");
        return;
    }

    int i = 0;

    for (; i < DIGITS; i++) {
        temp->v[i] = bigInt->v[i];
    }

    int carry = 0;
    uint64_t divisor = 1;

    for (; i < DIGITS; i++, divisor *= 10) {
        temp->v[i] += (n % 10) / divisor;
        temp->v[i] += carry;
        carry = temp->v[i] / 10;
        temp->v[i] %= 10;
    }

    if (overflow(temp)) {
        printf("Overflow encountered when adding.");
        free(temp);
        return;
    }

    free(bigInt);
    bigInt = temp;
}

void add_int256_t(int256_t* bigInt, const int256_t *restrict a) {
    if (!bigInt->positive ^ !a->positive) {
        printf("add_int256_t() only accepts int256_t's that are both positive or both negative.\n");
        return;
    }

    int256_t* temp = malloc(sizeof(int256_t));

    if (!temp) {
        printf("NULL pointer.");
        return;
    }
    
    int i = 0;

    for (; i < DIGITS; i++) {
        temp->v[i] = bigInt->v[i];
    }

    int carry = 0;

    for (i = 0; i < DIGITS; i++) {
        temp->v[i] += a->v[i];
        temp->v[i] += carry;
        carry = temp->v[i] / 10;
        temp->v[i] %= 10;
    }

    if (overflow(temp)) {
        printf("Overflow encountered when adding.");
        free(temp);
        return;
    }

    free(bigInt);
    bigInt = temp;
}

void display_int256_t(const int256_t *restrict bigInt) {
    if (!bigInt->positive) {
        printf("-");
    }

    int i = DIGITS - 1;

    while (!bigInt->v[i]) {
        i--;
    }

    for (; i >= 0; i--) {
        printf("%lu", bigInt->v[i]);
    }
    printf("\n"); // Remove later
}

bool overflow(const int256_t *restrict bigInt) {
    if (!bigInt) {
        printf("NULL int256_t encountered.\n");
        return true;
    }

    if (!bigInt->v[DIGITS - 1]) {
        return false;
    }

    int8_t MAX_SEQ[] = {
        5, 3, 9, 9, 3, 6, 9, 2, 1, 3, 1, 9, 7, 
        0, 0, 4, 8, 5, 7, 5, 4, 9, 3, 0, 4, 6, 
        5, 0, 4, 6, 5, 6, 6, 4, 8, 9, 9, 6, 2, 
        3, 5, 8, 7, 0, 9, 7, 8, 6, 8, 0, 0, 5, 
        8, 9, 0, 7, 5, 3, 2, 4, 5, 9, 1, 6, 1, 
        3, 7, 3, 2, 9, 8, 0, 2, 9, 7, 5, 1, 1
    };

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

void free_int256_t(void) {
    if (bigInts != NULL) {
        for (size_t i = 0; i < allocations; i++) {
            free(bigInts[i]);
        }
        free(bigInts);
    }
}
