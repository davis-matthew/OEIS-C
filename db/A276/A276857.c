/*
 * A276857  First differences of the Beatty sequence A022841 for sqrt(7).
 * https://oeis.org/A276857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(7)) - floor((n-1)*sqrt(7))
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9045 (2026-11-01)
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
    /* compute floor(n*sqrt(7)) as floor(sqrt(7*n*n)) */
    seq_t kn = (seq_t)n;            /* convert idx to seq_t */
    seq_t v1 = kn * kn;             /*   n^2  */
    v1 *= (seq_t)7;                 /*   7*n^2 */
    seq_t f1 = floor_sqrt(v1);      /*   floor(n*sqrt(7)) */

    /* compute floor((n-1)*sqrt(7)) */
    idx_t nm1 = n - 1ULL;           /* n-1; n>=1 guaranteed */
    seq_t kmn1 = (seq_t)nm1;
    seq_t v2 = kmn1 * kmn1;         /*   (n-1)^2  */
    v2 *= (seq_t)7;                 /*   7*(n-1)^2 */
    seq_t f2 = floor_sqrt(v2);      /*   floor((n-1)*sqrt(7)) */

    return f1 - f2;
}
