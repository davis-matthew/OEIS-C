/* digit_sum: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_H
#define OEISC_UTILS_DIGIT_SUM_H
#include <stdbool.h>
#include <stdint.h>

/* Compute the sum of the decimal digits of v. */
static seq_t digit_sum(seq_t v)
{
    seq_t s = 0;
    while (v > 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

#endif
