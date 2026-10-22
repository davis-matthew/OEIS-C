/*
 * A350599  Number of ways to partition the set of vertices of a convex n-gon
 *          into non-intersecting directed polygons.
 * https://oeis.org/A350599
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} 2^k * binomial(n+1, k) * *
 *            binomial(n-2*k-1, k-1) / (n+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;           /* undefined for n < 3, return 0 */

    seq_t numerator = 0;
    idx_t kmax = n / 3;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        seq_t term = pow2(k);                                    /* 2^k */
        term = term * binomial(n + 1, k);                        /* * binom(n+1,k) */
        term = term * binomial(n - 2 * k - 1, k - 1);            /* * binom(n-2k-1,k-1) */
        numerator += term;
    }

    seq_t denom = (seq_t)(n + 1);
    return numerator / denom;
}
