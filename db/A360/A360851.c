/*
 * A360851  Array read by antidiagonals: T(m,n) is the number of induced paths
 *          in the rook graph K_m X K_n.
 * https://oeis.org/A360851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where T(m,n) = -m*n + Σ_{j=1..min(m,n)}
 *            j!²·C(m,j)·C(n,j)·(1 + (m+n)/2 – j) with (m,n) derived from the
 *            antidiagonal reading of the array
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 112, u64 n <= 311,
 *            u128 n <= 799, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Determine diagonal index s so that prev = (s-1)*s/2 < n ≤ s*(s+1)/2 */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n)
        ++s;

    idx_t prev   = ((s - 1) * s) / 2;
    idx_t offset = n - prev;                /* 1 … s */

    idx_t m   = s - offset + 1;              /* row index   */
    idx_t col = offset;                      /* column index*/

    idx_t limit = (m < col) ? m : col;       /* min(m, col) */

    seq_t sum = 0;

    for (idx_t j = 1; j <= limit; ++j)
    {
        seq_t fact_j   = factorial(j);                                 /* j!            */
        seq_t term     = fact_j * fact_j;                              /* j!²           */
        term *= binomial(m,   j);                                     /* * C(m,j)      */
        term *= binomial(col, j);                                     /* * C(col,j)    */

        /* multiply by (m + col + 2 - 2*j) / 2   (exact division) */
        term *= (seq_t)(m + col + 2 - 2 * j);
        term /= 2;

        sum += term;
    }

    seq_t prod_mn = (seq_t)m * (seq_t)col;
    return sum - prod_mn;
}
