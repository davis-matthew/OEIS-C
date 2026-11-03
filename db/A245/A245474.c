/*
 * A245474  a(n) = smallest positive integer s such that s*n -
 *          floor(sqrt(s*n))^2 is a square.
 * https://oeis.org/A245474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer s such that s*n -
 *            floor(sqrt(s*n))^2 is a perfect square. (Direct search up to s =
 *            n.)
 * Range:     u8 n <= 10, u16 n <= 1011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9978 (2026-11-01)
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
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t N = (seq_t)n;             /* convert index to seq_t */
    seq_t s;

    for (s = 1; s <= (seq_t)n; ++s) {
        seq_t prod = N * s;          /* s * n */
        seq_t t    = floor_sqrt(prod);           /* floor(sqrt(s*n)) */
        seq_t diff = prod - t * t;                /* remainder */
        seq_t k    = floor_sqrt(diff);            /* sqrt of remainder */
        if (k * k == diff)                       /* remainder is a square? */
            return s;                            /* minimal s found */
    }

    /* In theory we should always return inside the loop.
       Fallback to n (s = n) if something unexpected occurs. */
    return (seq_t)n;
}
