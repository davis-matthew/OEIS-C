/*
 * A325004  Array read by descending antidiagonals: A(n,k) is the number of
 *          oriented colorings of the facets of a regular n-dimensional
 *          orthotope using up to k colors.
 * https://oeis.org/A325004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A(i,j) where (i,j) are the indices on the descending *
 *            antidiagonals of the array A(n,k) = C( C(k+1,2)+n-1, n ) + C(
 *            C(k,2), n ). * This implements the OEIS A325004 sequence
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 143, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Find the diagonal d (i+j), where d>=2 and
     * (d-1)d/2 >= n.  The diagonal length is d-1.
     */
    idx_t d = 2;                      /* start from diagonal 2 (i=j=1) */
    __int128 acc = 1;                 /* (d-1)*d/2 for d=2 is 1 */
    while (acc < n) {
        d++;
        acc = (__int128)(d - 1) * d / 2;
    }

    /* Compute i = n - C(d-2, d-1)  (1 <= i <= d-1) */
    idx_t prev = (idx_t)(((__int128)(d - 2) * (d - 1)) / 2);
    idx_t i = n - prev;               /* 1-based row index */
    idx_t j = d - i;                  /* 1-based color index */

    /* Compute binomial values needed by the formula.
     * For small j, the following products fit in 128 bits.
     */
    idx_t c1 = (idx_t)(((__int128)(j + 1) * j) / 2);   /* C(j+1,2) */
    idx_t c2 = (idx_t)(((__int128)j * (j - 1)) / 2);   /* C(j,2) */

    seq_t term1 = binom(c1 + i - 1, i);
    seq_t term2 = binom(c2, i);
    return term1 + term2;
}
