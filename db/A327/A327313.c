/*
 * A327313  Positions of 2's in {A327310(n) : n > 0}.
 * https://oeis.org/A327313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th positive integer k with floor(3*k*sqrt(8)) -
 *            3*floor(k*sqrt(8)) = 2; found by scanning k upward
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 1119, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9922 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A327313_isqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    /* invariant: lo*lo <= x < hi*hi */
    while (hi - lo > 1)
    {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t k = 0;
    while (count < target)
    {
        k++;
        seq_t f1 = A327313_isqrt((seq_t)8 * k * k);
        seq_t f2 = A327313_isqrt((seq_t)72 * k * k);
        if (f2 - (seq_t)3 * f1 == (seq_t)2)
            count++;
    }
    return k;
}
