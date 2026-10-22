/*
 * A381862  Number of pairs of triangles that are pairwise edge-disjoint in
 *          the complete graph K_n.
 * https://oeis.org/A381862
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n+4)/72
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the helper functions available in the utilities package. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 5) the value is undefined. */
    if (n < 5) return 0;

    /* 10 * C(n,6) */
    seq_t term1 = 10 * binomial(n, 6);

    /* 3 * n * C(n-1,4) */
    seq_t term2 = 3 * (seq_t)n * binomial(n - 1, 4);

    return term1 + term2;
}
