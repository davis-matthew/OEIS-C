/*
 * A327311  Positions of 0's in {A327310(n) : n > 0}.
 * https://oeis.org/A327311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of n-th k>=1 with floor(3*k*sqrt(8)) -
 *            3*floor(k*sqrt(8)) = 0; computed by a linear scan
 * Range:     u8 none, u16 n <= 10, u32 n <= 3273, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8674 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* largest r in [lo, hi] with r*r <= x */
static seq_t A327311_fsqrt(seq_t x, seq_t lo, seq_t hi)
{
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        k++;
        seq_t s = (seq_t)k;
        seq_t f1 = A327311_fsqrt((seq_t)8 * s * s, (seq_t)2 * s, (seq_t)3 * s);
        seq_t f2 = A327311_fsqrt((seq_t)72 * s * s, (seq_t)8 * s, (seq_t)9 * s);
        if (f2 == (seq_t)3 * f1)
            count++;
    }
    return (seq_t)k;
}
