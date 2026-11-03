/*
 * A318996  a(n) = Sum_{d|n} (sigma(n) mod d).
 * https://oeis.org/A318996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (sigma(n) mod d)
 * Range:     u8 n <= 63, u16 n <= 9414, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 88 terms through n = 9414 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    seq_t sigma = sigma_of(n);
    seq_t sum = 0;

    idx_t root = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d == 0)
        {
            sum += sigma % (seq_t)d;
            idx_t other = n / d;
            if (other != d)
                sum += sigma % (seq_t)other;
        }
    }
    return sum;
}
