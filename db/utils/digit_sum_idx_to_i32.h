/* digit_sum_idx_to_i32: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_IDX_TO_I32_H
#define OEISC_UTILS_DIGIT_SUM_IDX_TO_I32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: sum of decimal digits of an integer of type idx_t. */
static inline int digit_sum_idx_to_i32(idx_t x)
{
    int s = 0;
    while (x > 0) {
        s += (int)(x % 10);
        x /= 10;
    }
    return s;
}

#endif
