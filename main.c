#include <stdio.h>
#include <stdlib.h>
#include "int256_t.h"

int main(void) {
    atexit(free_int256_t);

    int256_t* bigInt = decl_int256_t();

    if (!bigInt) {
        printf("NULL pointer.");
        return 1;
    }

    set_n(bigInt, 1);

    // display_int256_t(bigInt);

    // int256_t* otherInt = decl_int256_t();

    // if (!otherInt) {
    //     printf("NULL pointer.");
    //     return 1;
    // }

    // set_int256_t(otherInt, bigInt);

    // int256_t* temp = multiply_n(otherInt, 5);

    // if (!temp) {
    //     printf("NULL pointer.");
    //     return 1;
    // }

    // display_int256_t(otherInt);
    // display_int256_t(temp);

    // clear(temp);

    // display_int256_t(temp);

    for (int i = 1; i <= 256; i++) {
        bigInt = multiply_n(bigInt, 2);
        printf("%i: ", i);
        display_int256_t(bigInt);
    }

    return 0;
}
