/* digit_sum_u64: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_U64_H
#define OEISC_UTILS_DIGIT_SUM_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--------- helper: digit sum of a 64‑bit integer ---------------------*/
static uint64_t digit_sum_u64(uint64_t x)
{
    uint64_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

#endif
