/*
 * A327209  Positions of 0's in {A327208(n) : n > 0}.
 * https://oeis.org/A327209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th k >= 1 with A327208(k) = 0, where
 *            A327208(k) = floor((2k+2)*sqrt(3)) - floor((k+2)*sqrt(3)) -
 *            floor(k*sqrt(3)); computed by linear scan
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9355 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A327209_fl(uint64_t m)
{
    /* floor(m*sqrt(3)) = isqrt(3*m*m) */
    uint64_t x = 3 * m * m;
    uint64_t lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n)
    {
        k++;
        uint64_t v = A327209_fl(2 * k + 2) - A327209_fl(k + 2) - A327209_fl(k);
        if (v == 0)
            count++;
    }
    return (seq_t)k;
}
