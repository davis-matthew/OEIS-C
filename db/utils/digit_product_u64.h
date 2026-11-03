/* digit_product_u64: the product of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_PRODUCT_U64_H
#define OEISC_UTILS_DIGIT_PRODUCT_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Compute the product of the decimal digits of x (x fits in 64‑bit for the bound). */
static unsigned long long digit_product_u64(unsigned long long x)
{
    if (x == 0) return 0;
    unsigned long long prod = 1;
    while (x) {
        prod *= x % 10;
        x /= 10;
    }
    return prod;
}

#endif
