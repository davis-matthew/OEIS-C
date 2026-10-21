/*
 * A298950  Numbers k such that 5*k - 4 is a square.
 * https://oeis.org/A298950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n*(n-1) + (2*n-1)*(-1)^n + 9)/8
 * Range:     u8 n <= 14, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If called with n=0, the value is irrelevant, return 0. */
    if (n == 0) return 0;

    /* Use signed 64‑bit arithmetic for the small intermediate values. */
    int64_t nn = (int64_t)n;

    int64_t term1 = 10LL * nn * (nn - 1);          /* 10*n*(n-1) */
    int64_t term2 = (2LL * nn - 1) * ((nn & 1ULL) ? -1L : 1L); /* (2*n-1)*(-1)^n */
    int64_t num   = term1 + term2 + 9;              /* numerator */

    /* The numerator is always divisible by 8 for n>=1. */
    return (seq_t)(num / 8);
}
