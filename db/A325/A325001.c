/*
 * A325001  Array read by descending antidiagonals: A(n,k) is the number of
 *          achiral colorings of the facets (or vertices) of a regular
 *          n-dimensional simplex using up to k colors.
 * https://oeis.org/A325001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(j+1,i+1) - binom(j-i+1,i+1) where j =
 *            ceil((sqrt(8n+1)-1)/2), i = n - j(j-1)/2
 * Range:     u8 n <= 47, u16 n <= 159, u32 n <= 576, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 82 terms through n = 1196 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, indexing starts at 1 */

    /* find j such that j*(j+1)/2 >= n */
    idx_t j = 1;
    while ((j * (j + 1)) / 2 < n) ++j;

    /* compute the row index i within column j */
    idx_t i = n - ((j - 1) * j) / 2;  /* 1 <= i <= j */

    /* A(i,k) with k = j - i + 1 is binom(j+1,i+1) - binom(j-i+1,i+1) */
    seq_t term1 = binomial(j + 1, i + 1);
    seq_t term2 = binomial(j - i + 1, i + 1);

    return term1 - term2;
}
