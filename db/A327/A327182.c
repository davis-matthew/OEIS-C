/*
 * A327182  Positions of 1's in {A327180(n) : n > 0}.
 * https://oeis.org/A327182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327180(k) = 1, where A327180(k) =
 *            floor((2k+1)*sqrt(3)) - floor((k+1)*sqrt(3)) - floor(k*sqrt(3));
 *            found by a linear scan
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 4244, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9462 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Floor square root by binary search */
static seq_t A327182_floor_sqrt(seq_t x)
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

static seq_t A327182_fl3(seq_t k)
{
    return A327182_floor_sqrt((seq_t)3 * k * k);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t k = 0;
    seq_t fk = 0; /* floor(k*sqrt(3)) */
    seq_t target = (seq_t)n;
    while (count < target)
    {
        seq_t fk1 = A327182_fl3(k + 1);
        seq_t f2 = A327182_fl3(2 * k + 1);
        k++;
        /* A327180(k-1) is computed with the previous k; we need A327180(k) */
        (void)f2;
        (void)fk;
        fk = fk1;
        /* value at current k */
        seq_t v = A327182_fl3(2 * k + 1) - A327182_fl3(k + 1) - fk;
        if (v == 1)
            count++;
    }
    return k;
}
