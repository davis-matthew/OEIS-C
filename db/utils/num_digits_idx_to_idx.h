/* num_digits_idx_to_idx: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_IDX_TO_IDX_H
#define OEISC_UTILS_NUM_DIGITS_IDX_TO_IDX_H
#include <stdint.h>

/* number of decimal digits in n; n==0 has 1 digit */
static idx_t num_digits_idx_to_idx(idx_t n)
{
    if (n == 0) return 1;
    idx_t d = 0;
    while (n != 0) {
        ++d;
        n /= 10;
    }
    return d;
}

#endif
