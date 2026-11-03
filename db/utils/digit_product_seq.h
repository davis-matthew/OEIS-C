/* digit_product_seq: the product of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_PRODUCT_SEQ_H
#define OEISC_UTILS_DIGIT_PRODUCT_SEQ_H
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

/* compute product of decimal digits of x; return 0 if any digit is 0 */
static uint64_t digit_product_seq(seq_t x)
{
    if (x == (seq_t)0) return 0;
    uint64_t prod = 1;
    uint64_t v = (uint64_t)x;
    while (v > 0) {
        uint64_t d = v % 10;
        if (d == 0) return 0;
        prod *= d;
        v /= 10;
    }
    return prod;
}

#endif
