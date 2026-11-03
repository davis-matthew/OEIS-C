/*
 * A364105  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(5*k-1)).
 * https://oeis.org/A364105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Σ_{d|5*n-2, d≡4 (mod 5)} (d+1)
 * Range:     u8 n <= 13, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-01)
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
    /* Compute m = 5*n - 2 as a seq_t to avoid overflow. */
    seq_t m = (seq_t)5 * n - (seq_t)2;
    seq_t sum = 0;

    /* The largest possible divisor to search is sqrt(m). */
    seq_t lim = floor_sqrt(m);

    for (seq_t i = 1; i <= lim; ++i)
    {
        if (m % i == 0)
        {
            seq_t d = i;
            seq_t e = m / i;

            if (d % 5 == 4)
                sum += d + 1;

            if (e != d && e % 5 == 4)
                sum += e + 1;
        }
    }

    return sum / 5;
}
