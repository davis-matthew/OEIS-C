/*
 * A364065  Expansion of Sum_{k>0} k * x^(2*k-1) / (1 - x^(3*k-2)).
 * https://oeis.org/A364065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * sum_{d | (3*n-1), d==1 (mod 3)} (d+2)
 * Range:     u8 n <= 21, u16 n <= 5249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9635 (2026-11-01)
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
    /* Compute m = 3*n - 1 */
    idx_t m = 3 * n - 1;

    seq_t sum = 0;

    /* floor_sqrt returns seq_t; use it as a bound for the loop */
    idx_t limit = (idx_t)floor_sqrt((seq_t)m);

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (m % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = m / d;

            if (d1 % 3 == 1)
                sum += (seq_t)d1 + 2;

            if (d2 != d1 && d2 % 3 == 1)
                sum += (seq_t)d2 + 2;
        }
    }

    return sum / 3;
}
