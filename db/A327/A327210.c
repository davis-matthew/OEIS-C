/*
 * A327210  Positions of 1's in {A327208(n) : n > 0}.
 * https://oeis.org/A327210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th... i.e. a(n) = least k such that exactly n values j
 *            in 1..k have A327208(j) = 1, where A327208(j) =
 *            floor((2j+2)*sqrt(3)) - floor((j+2)*sqrt(3)) - floor(j*sqrt(3));
 *            found by a linear scan
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9212 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor(k*sqrt(3)) = floor_sqrt(3*k^2), by binary search */
static seq_t A327210_fl(idx_t k)
{
    seq_t t = (seq_t)3 * (seq_t)k * (seq_t)k;
    seq_t lo = 0, hi = (seq_t)2 * (seq_t)k + 1;
    while (hi - lo > 1)
    {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= t)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t limit = 4 * n + 100;
    idx_t k;
    for (k = 1; k <= limit; k++)
    {
        seq_t v = A327210_fl(2 * k + 2) - A327210_fl(k + 2) - A327210_fl(k);
        if (v == 1)
        {
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
    return (seq_t)k;
}
