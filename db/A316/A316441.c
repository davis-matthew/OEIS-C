/*
 * A316441  a(n) = Sum (-1)^k where the sum is over all factorizations of n
 *          into factors > 1 and k is the number of factors.
 * https://oeis.org/A316441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{n = ∏ f_i, f_i > 1} (-1)^{#factors}
 * Range:     i8 n <= 9972, i16 n <= 9972, i32 n <= 9972, i64 n <= 9972,
 *            i128 n <= 9972, big any n
 * Helpers:   factor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all multiplicative factorizations
   in non‑decreasing order and sums the signs of each factorization. */

#include <stdint.h>
#include "../utils/factor_sum.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) = 1 */
    if (n == 1) {
        return 1;
    }
    return factor_sum((uint64_t)n, 2, 0);
}
