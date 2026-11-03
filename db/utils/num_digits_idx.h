/* num_digits_idx: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_IDX_H
#define OEISC_UTILS_NUM_DIGITS_IDX_H
#include <stdbool.h>
#include <stdint.h>

/* Helper: number of decimal digits of an unsigned integer. */
static int num_digits_idx(idx_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

#endif
