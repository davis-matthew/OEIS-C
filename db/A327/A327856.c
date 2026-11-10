/*
 * A327856  The upper left triangular section of square array A091255, read by
 *          rows.
 * https://oeis.org/A327856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A091255(r,k) = gcd over GF(2) of the polynomials with
 *            binary coefficients r and k, where r is the row of n in the
 *            triangle and k = n - r(r-1)/2
 * Range:     u8 n <= 10440, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 121 terms through n = 9942 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned int A327856_deg(uint64_t p)
{
    unsigned int d = 0;
    while (p >>= 1)
        d++;
    return d;
}

static uint64_t A327856_gcd2(uint64_t x, uint64_t y)
{
    while (y != 0)
    {
        unsigned int dy = A327856_deg(y);
        while (x != 0 && A327856_deg(x) >= dy)
            x ^= y << (A327856_deg(x) - dy);
        uint64_t t = x;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    /* smallest r with r(r+1)/2 >= N */
    uint64_t lo = 1, hi = 6000000000ULL;
    while (lo < hi)
    {
        uint64_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= N)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t r = lo;
    uint64_t k = N - r * (r - 1) / 2;
    return (seq_t)A327856_gcd2(r, k);
}
