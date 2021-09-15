#include <stdio.h>
#include <stdlib.h>
#include "int256_t.h"

int main(void) {
    int256_t* bigInt = decl_int256_t();

    if (!bigInt) {
        printf("NULL pointer.");
        return 1;
    }

    bigInt = init_n(1);

    for (int i = 1; i <= 50; i++) {
        bigInt = multiply_n(bigInt, i);
    }

    printf("Factorial of 50:\n");
    display_int256_t(bigInt);

    set_int256_t(bigInt, to_int256_t(1));

    for (int i = 1; i <= 256; i++) {
        bigInt = multiply_n(bigInt, 2);
    }

    printf("\n\n2^256:\n");
    display_int256_t(bigInt);

    return 0;
}
