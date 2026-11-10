/*
 * A327221  Positions of 1's in {A327219(k) : n > 0}.
 * https://oeis.org/A327221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327219(k) = 1, where A327219(k) =
 *            floor((2k+4)r) - floor((k+4)r) - floor(k*r), r = (1+sqrt(5))/2;
 *            found by scanning k upward
 * Range:     u8 none, u16 n <= 28, u32 n <= 3594, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9391 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A327221_floor_sqrt(seq_t x)
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

static seq_t A327221_floor_nphi(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t = ns * ns * (seq_t)5;
    seq_t r = A327221_floor_sqrt(t);
    return (ns + r) / 2;
}

static seq_t A327221_f(idx_t k)
{
    seq_t fn = A327221_floor_nphi(k);
    seq_t fn4 = A327221_floor_nphi(k + 4);
    seq_t f2n4 = A327221_floor_nphi((k << 1) + 4);
    return f2n4 - fn4 - fn;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        k++;
        if (A327221_f(k) == 1)
            count++;
    }
    return (seq_t)k;
}
