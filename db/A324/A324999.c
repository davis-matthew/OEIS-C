/*
 * A324999  Array read by descending antidiagonals: A(n,k) is the number of
 *          oriented colorings of the facets (or vertices) of a regular
 *          n-dimensional simplex using up to k colors.
 * https://oeis.org/A324999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = binomial(n+k,n+1)+binomial(k,n+1); a(n) is obtained by
 *            reading the array A(n,k) along descending antidiagonals
 *            (starting with (1,1), then (1,2),(2,1), etc.). The index mapping
 *            is: let s be the smallest integer with s*(s-1)/2 >= n; let
 *            offset = n - (s-1)*(s-2)/2; then (n,k) = (offset, s-o...
 * Range:     u8 n <= 23, u16 n <= 158, u32 n <= 539, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find diagonal s such that C(s) = s*(s-1)/2 >= n */
    idx_t s = 2;
    /* Use 128‑bit multiplication to avoid overflow for very large n */
    while ( ( (__uint128_t)s * (s - 1) / 2 ) < n )
        ++s;

    /* C(s-1) = (s-1)*(s-2)/2 */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t offset = n - prev;           /* 1‑based position on diagonal */

    idx_t n_row = offset;              /* row index (n) */
    idx_t k_col = s - offset;          /* column index (k) */

    /* Compute A(n,k) = binomial(n+k,n+1) + binomial(k,n+1) */
    seq_t term1 = binomial(s, n_row + 1);   /* n + k = s */
    seq_t term2 = binomial(k_col, n_row + 1);

    return term1 + term2;
}
