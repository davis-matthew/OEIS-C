/*
 * A325003  Triangle read by rows: T(n,k) is the number of achiral colorings
 *          of the facets (or vertices) of a regular n-dimensional simplex
 *          using exactly k colors.
 * https://oeis.org/A325003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r, c-1) - [c == r+1] where (r,c) is obtained by
 *            reading the triangle T(n,k) = binomial(n, k-1) – [k == n+1]
 *            row‑by‑row. The sequence starts with a(1)
 * Range:     u8 n <= 39, u16 n <= 196, u32 n <= 573, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; safety for n==0 */
        return 0;

    idx_t r = 0;           /* current row number */
    idx_t cum = 0;         /* cumulative count of terms up to current row */

    /* find the row r that contains the n-th term */
    do {
        ++r;
        cum += r + 1;      /* current row has r+1 terms */
    } while (cum < n);

    /* number of terms before this row */
    idx_t prev_cum = cum - (r + 1);
    idx_t col = n - prev_cum;    /* 1‑based column index within the row */

    if (col == r + 1)            /* the last term of every row is 0 */
        return 0;

    return binomial(r, col - 1);
}
