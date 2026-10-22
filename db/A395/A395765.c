/*
 * A395765  First column of the triangular array with T(0, m) = m! and T(n, m)
 *          = T(n - 1, m + 3) - T(n - 1, m + 2).
 * https://oeis.org/A395765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j * binomial(n, j) * (3*n - j)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;

    /* Compute (3*n)! */
    idx_t mulN = 3 * n;            /* 3*n fits in idx_t for the intended usage */
    seq_t fact   = (seq_t)1;
    for (idx_t i = 1; i <= mulN; ++i)
        fact *= (seq_t)i;          /* factorial iteratively */

    seq_t currFact = fact;         /* (3*n - j)! for j = 0 */
    seq_t binom    = (seq_t)1;     /* binomial(n,0) */
    int   sign     = 1;            /* (+1) for j even, (-1) for j odd */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom * currFact;
        if (sign > 0)
            result += term;
        else
            result -= term;

        if (j == n) break;         /* avoid one more update after last term */

        /* Update binomial coefficient:
           binom(n, j+1) = binom(n, j) * (n - j) / (j + 1) */
        binom *= (seq_t)(n - j);
        binom /= (seq_t)(j + 1);

        /* Update factorial:
           (3*n - (j+1))! = (3*n - j)! / (3*n - j) */
        currFact /= (seq_t)(mulN - j);

        sign = -sign;              /* flip sign for next j */
    }

    return result;
}
