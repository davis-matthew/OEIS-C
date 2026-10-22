/*
 * A325006  Array read by descending antidiagonals: A(n,k) is the number of
 *          chiral pairs of colorings of the facets of a regular n-dimensional
 *          orthotope using up to k colors.
 * https://oeis.org/A325006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial( binomial(k, 2), n ) where (n,k) are obtained
 *            from the index n by reading the array
 *            A(n,k)=binom(binomial(k,2),n) along descending antidiagonals:
 *            find the smallest d such that (d-1)d/2 ≥ n, let offset = n -
 *            (d-2)(d-1)/2 - 1, then n = offset + 1 and k = d - offset - 1
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 161, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1, but guard against 0 */

    /* Find d such that (d-1)d/2 >= n */
    idx_t d = 2;
    while (d * (d - 1) / 2 < n) d++;

    /* Number of elements before this diagonal */
    idx_t prev = (d - 2) * (d - 1) / 2;

    /* Position within diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Coordinates (nCoord,kCoord) */
    idx_t nCoord = offset + 1;
    idx_t kCoord = d - offset - 1;

    /* binom(kCoord,2) */
    if (kCoord < 2) return (seq_t)0;
    seq_t t = (seq_t)kCoord * ((seq_t)kCoord - 1) / 2;

    /* binom(t, nCoord) */
    if (t < (seq_t)nCoord) return (seq_t)0;
    return binomial((idx_t)t, nCoord);
}
