/* digit_sum_u64_to_i32: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_U64_TO_I32_H
#define OEISC_UTILS_DIGIT_SUM_U64_TO_I32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Digit sum of an unsigned 64‑bit integer */
static int digit_sum_u64_to_i32(uint64_t x)
{
    int s = 0;
    while (x) { s += x % 10; x /= 10; }
    return s;
}

#endif
