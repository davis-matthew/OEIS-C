/*
 * A287726  Positions of 0 in A287725; complement of A287727.
 * https://oeis.org/A287726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi) + 3*n - 2 where phi = (1 + sqrt(5)) / 2. The
 *            integer part of n*phi can be computed without floating point as
 *            floor(n*phi) = floor((n + floor(n*sqrt(5))) / 2)
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9309 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0
       we return 0, though this case will not be used. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    /* Compute floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t t = N * N;   /* n^2 */
    t *= 5;            /* 5*n^2 */
    seq_t s = floor_sqrt(t); /* floor(n*sqrt(5)) */

    /* Compute floor(n*phi) = floor((n + s) / 2)  */
    seq_t v = (N + s) / 2;

    /* Return the final value */
    return v + 3 * N - 2;
}
