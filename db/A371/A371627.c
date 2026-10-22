/*
 * A371627  The x-coordinate of the point where x + y = n, x is an integer and
 *          x/y is as close as possible to 1/phi.
 * https://oeis.org/A371627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = floor(n / φ²) or ceil(n / φ²), whichever yields a ratio
 *            x/(n‑x) closest to 1/φ
 * Range:     u8 n <= 668, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ = (1+√5)/2.  The sequence is defined for n ≥ 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined by OEIS, but harmless guard */

    /* Numerical constants based on a rational approximation of √5:
       √5 ≈ 2236067977499 / 10¹².  The approximation is accurate to
       ~4·10⁻¹³, which is far below the resolution required for n ≤ 10⁴. */
    const int64_t sqrt5_num = 2236067977499LL;        /* numerator of √5   */
    const int64_t sqrt5_den = 1000000000000LL;        /* denominator of √5 */

    /* 1 / φ = (√5 − 1) / 2 */
    const int64_t r_num = sqrt5_num - sqrt5_den;      /* numerator of 1/φ   */
    const int64_t r_den = 2 * sqrt5_den;              /* denominator of 1/φ */

    /* 1 / φ² = (3 − √5) / 2 */
    const int64_t inv_num = 3 * sqrt5_den - sqrt5_num;/* numerator of 1/φ²  */
    const int64_t inv_den = 2 * sqrt5_den;            /* denominator of 1/φ² */

    /* Candidate floor value: x₀ = ⌊ n / φ² ⌋ */
    __int128 prod = (__int128)n * inv_num;
    uint64_t x0 = (uint64_t)(prod / inv_den);
    if (x0 > n) x0 = n;                     /* safety, should not happen */

    /* Compute |x/(n−x) − 1/φ| numerators for x₀ and x₀+1 */
    uint64_t y0 = n - x0;
    __int128 N0 = (__int128)x0 * r_den - (__int128)r_num * y0;
    if (N0 < 0) N0 = -N0;                     /* absolute value */

    uint64_t best = x0;                       /* assume floor is best */

    if (x0 < n) {                            /* candidate x₁ = x₀ + 1 exists */
        uint64_t x1 = x0 + 1;
        uint64_t y1 = n - x1;
        __int128 N1 = (__int128)x1 * r_den - (__int128)r_num * y1;
        if (N1 < 0) N1 = -N1;

        /* Compare |N0|/(y0)  with  |N1|/(y1)  without division:
           choose x₁ if N1/y1 < N0/y0, i.e. N1 * y0 < N0 * y1.             */
        __int128 lhs = N0 * y1;             /* N0 * (n−x₀−1) */
        __int128 rhs = N1 * y0;             /* N1 * (n−x₀)   */

        if (rhs < lhs) best = x1;
    }

    return (seq_t)best;
}
