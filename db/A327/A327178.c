/*
 * A327178  Positions of 0's in {A327177(n) : n > 0}.
 * https://oeis.org/A327178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive k with floor((2k+1)*sqrt(2)) -
 *            floor((k+1)*sqrt(2)) - floor(k*sqrt(2)) = 0; found by linear
 *            search
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9364 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor(k*sqrt(2)) = largest m with m*m <= 2*k*k, by binary search in [k, 2k) */
static seq_t A327178_fl(seq_t k)
{
    seq_t x = (seq_t)2 * k * k;
    seq_t lo = k, hi = (seq_t)2 * k;
    while (hi - lo > 1)
    {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    for (;;)
    {
        k++;
        seq_t kk = (seq_t)k;
        seq_t v = A327178_fl(2 * kk + 1) - A327178_fl(kk + 1) - A327178_fl(kk);
        if (v == 0)
        {
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
}
