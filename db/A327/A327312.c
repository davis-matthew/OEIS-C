/*
 * A327312  Positions of 1's in {A327310(n) : n > 0}.
 * https://oeis.org/A327312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th positive integer k with A327310(k) = 1, where
 *            A327310(k) = floor(3*k*sqrt(8)) - 3*floor(k*sqrt(8)); found by a
 *            linear scan
 * Range:     u8 none, u16 n <= 10, u32 n <= 310, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9816 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A327312_floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t A327312_A327310(idx_t k)
{
    seq_t s = (seq_t)k;
    seq_t f1 = A327312_floor_sqrt((seq_t)8 * s * s);
    seq_t s6 = s * (seq_t)6;
    seq_t f2 = A327312_floor_sqrt((seq_t)2 * s6 * s6);
    return f2 - (seq_t)3 * f1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        k++;
        if (A327312_A327310(k) == (seq_t)1)
            count++;
    }
    return (seq_t)k;
}
