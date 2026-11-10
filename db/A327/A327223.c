/*
 * A327223  Positions of 0's in {A327222(k) : n > 0}.
 * https://oeis.org/A327223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327222(k) = 0, where A327222(k) =
 *            floor((2k+4)*sqrt(2)) - floor((k+4)*sqrt(2)) - floor(k*sqrt(2));
 *            found by a linear search
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8673 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor(m*sqrt(2)) = largest x with x*x <= 2*m*m, by binary search in [m, 2m] */
static seq_t A327223_fl(seq_t m)
{
    seq_t target = (seq_t)2 * m * m;
    seq_t lo = m, hi = (seq_t)2 * m;
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= target)
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
        seq_t ks = (seq_t)k;
        seq_t t = A327223_fl((seq_t)2 * ks + 4);
        seq_t u = A327223_fl(ks + 4);
        seq_t v = A327223_fl(ks);
        if (t == u + v)
            count++;
    }
    return (seq_t)k;
}
