/*
 * A327306  a(n) = floor(3*n*r) - 3*floor(n*r), where r = sqrt(6).
 * https://oeis.org/A327306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n*sqrt(6)) - 3*floor(n*sqrt(6))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of an unsigned __int128.
   Returns the largest r such that r*r <= x. */
static inline unsigned __int128 isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64);   /* upper bound for sqrt(2^128-1) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    unsigned __int128 n128   = (unsigned __int128)n;
    unsigned __int128 n2     = n128 * n128;

    /* floor(n*sqrt(6))  = floor(sqrt(6*n^2)) */
    unsigned __int128 six_n2   = 6 * n2;
    unsigned __int128 floor_n  = isqrt_uint128(six_n2);

    /* floor(3*n*sqrt(6))  = floor(sqrt(54*n^2)) */
    unsigned __int128 fiftyfour_n2 = 54 * n2;
    unsigned __int128 floor_3n = isqrt_uint128(fiftyfour_n2);

    /* The result is small (0,1,2), cast to seq_t safely. */
    return (seq_t)(floor_3n - 3 * floor_n);
}
