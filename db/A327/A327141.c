/*
 * A327141  a(n) is the number of different sizes of integer-sided rectangles
 *          which can be placed inside an n X n square.
 * https://oeis.org/A327141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 + Σ_{a=n+1}^{⌊n·√2⌋} min(a, ⌊n·√2⌋ – a)
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum counts integer-sided rectangles with a side > n that still fit
   into an n×n square by rotation. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* triangular part A000217(n) */
    seq_t result = (seq_t)n * ((seq_t)n + 1) / 2;

    /* L = floor(n*√2) = floor(√(2*n^2)) */
    seq_t L = floor_sqrt( (seq_t)2 * (seq_t)n * (seq_t)n );

    /* Count extra rectangles with a > n */
    for (idx_t a_n = n + 1; a_n <= (idx_t)L; ++a_n) {
        seq_t a_big = (seq_t)a_n;
        seq_t m     = L - a_big;          /* remaining sum for b */
        if (m == 0) break;                /* no further pairs possible */

        /* count of b's: 1 .. min(a, m) */
        seq_t cnt = (a_big < m) ? a_big : m;
        result += cnt;
    }
    return result;
}
