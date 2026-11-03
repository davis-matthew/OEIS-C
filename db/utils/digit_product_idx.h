/* digit_product_idx: the product of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_PRODUCT_IDX_H
#define OEISC_UTILS_DIGIT_PRODUCT_IDX_H
#include <stdint.h>
#include <stdlib.h>

/* Return product of decimal digits of x (x fits in idx_t). */
static seq_t digit_product_idx(idx_t x)
{
    if (x == 0) return 0;
    unsigned long long prod = 1;
    while (x > 0)
    {
        prod *= (x % 10);
        x /= 10;
    }
    return (seq_t)prod;
}

#endif
