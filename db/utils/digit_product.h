/* digit_product: the product of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_PRODUCT_H
#define OEISC_UTILS_DIGIT_PRODUCT_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Product of decimal digits of x.
   If any digit is zero the product is zero. */
static seq_t digit_product(seq_t x)
{
    if (x == 0)
        return 0;
    seq_t prod = 1;
    while (x > 0)
    {
        seq_t d = x % 10;
        if (d == 0)
            return 0;          /* early exit */
        prod *= d;
        x /= 10;
    }
    return prod;
}

#endif
