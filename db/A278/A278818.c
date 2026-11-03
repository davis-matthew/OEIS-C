/*
 * A278818  a(n) is the least k > n such that k + n is square.
 * https://oeis.org/A278818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(2n+1))^2 - n
 * Range:     u8 n <= 31, u16 n <= 7950, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9777 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* compute v = 2n + 1 */
    seq_t v = (seq_t)n * 2 + 1;

    /* floor square root of v */
    seq_t s = floor_sqrt(v);

    /* ceil square root: increase if not a perfect square */
    if (s * s < v) {
        s++;
    }

    /* square of the ceil root */
    seq_t sq = s * s;

    /* result = sq - n */
    return sq - n;
}
