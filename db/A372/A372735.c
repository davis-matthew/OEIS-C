/*
 * A372735  Number of distinct circles that can be constructed from the 3
 *          vertices and the equally spaced 3*n points placed on the sides of
 *          an equilateral triangle when every pair of the 3 + 3*n points are
 *          connected by a circle and where the points lie at the ends of the
 *          circle's diameter.
 * https://oeis.org/A372735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = C(3(n+1), 2) – (12 + Σ_{k=2}^{n} g(k)), where g(k) is a
 *            6‑periodic sequence g(k) = {8,13,9,11,10,12} indexed by k mod 6.
 *            This yields an O(1) algorithm
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* sequence and index types are supplied by the build system */
static const idx_t G[6] = { 8, 13, 9, 11, 10, 12 };

seq_t a(idx_t n)
{
    if (n == 0U)          /* offset starts at 1 (unused case) */
        return 0;

    /* number of points: N = 3(n+1) */
    seq_t N = (seq_t)3 * ((seq_t)n + 1);

    /* total unordered pairs C(N,2) without overflow   */
    seq_t total_pairs;
    if (N & 1U) {                 /* N odd: (N-1)/2 is integer */
        total_pairs = N * ((seq_t)(N - 1) / 2);
    } else {                      /* N even: N/2 is integer   */
        total_pairs = (N / 2) * (seq_t)(N - 1);
    }

    /* duplicate circles count d(n) */
    seq_t d = 12;                      /* base value for n = 1 */
    if (n > 1U) {
        idx_t t   = n - 1U;            /* number of terms from k = 2 to n */
        idx_t full = t / 6U;           /* full 6‑term cycles */
        idx_t rem  = t % 6U;           /* remainder terms */

        d += (seq_t)full * 63;        /* 63 = sum of G[0..5] */

        /* accumulate the remaining g(k) values */
        for (idx_t i = 0; i < rem; ++i)
            d += (seq_t)G[(2U + i) % 6U];
    }

    return total_pairs - d;
}
