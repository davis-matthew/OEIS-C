/*
 * A347526  Number of divisors of n that are at most n^(1/4).
 * https://oeis.org/A347526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are at most n^(1/4)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 9894 (2026-11-01)
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

/* Count divisors d of n with d ≤ n^(1/4) */
seq_t a(idx_t n)
{
    /* Compute the integer part of the fourth root of n */
    const idx_t r1 = (idx_t)floor_sqrt((seq_t)n);   /* sqrt(n)   */
    const idx_t r4 = (idx_t)floor_sqrt((seq_t)r1);  /* sqrt(sqrt(n)) */

    seq_t count = 0;
    for (idx_t d = 1; d <= r4; ++d) {
        if (n % d == 0) {
            ++count;
        }
    }
    return count;
}
