/* digit_sum_idx: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_IDX_H
#define OEISC_UTILS_DIGIT_SUM_IDX_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t digit_sum_idx(idx_t n)
{
    idx_t s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

#endif
