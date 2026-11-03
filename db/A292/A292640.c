/*
 * A292640  Rank of (4-r)*n when all the numbers (4-r)*j and (4+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((9 - 4√2)/7 * n)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 982 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the integer square‑root floor_sqrt to compute floor(4*n*√2). */
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
        return 0;   /* sequence is defined for n ≥ 1 */

    /* Compute b = floor(4*n*sqrt(2)) = floor(sqrt(2*(4n)^2)) */
    seq_t m = (seq_t)4 * (seq_t)n;
    seq_t b = floor_sqrt((seq_t)2 * m * m);

    /* t = 9*n - floor(4*n*sqrt(2)) */
    seq_t t = (seq_t)9 * (seq_t)n - b;

    /* k = floor(t/7) minus 1 if t is divisible by 7 */
    seq_t q = t / (seq_t)7;
    seq_t r = t % (seq_t)7;
    seq_t k = q - (r == 0 ? (seq_t)1 : (seq_t)0);

    /* return a(n) = n + k */
    return (seq_t)n + k;
}
