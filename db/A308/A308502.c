/*
 * A308502  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} d^(n/d + k).
 * https://oeis.org/A308502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{d|n1} d^(n1/d + k) where (n1,k) are obtained from n
 *            by reading the square array A(n,k) by antidiagonals: for the
 *            smallest integer s with s*(s+1)/2 ≥ n, let prev = (s-1)*s/2,
 *            within = n - prev - 1. Then n1 = within + 1 (1‑based row) and k
 *            = s - n1
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal (s) that contains the index n */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n)
        ++s;

    /* Position inside that diagonal */
    idx_t prev     = (s - 1) * s / 2;          /* total entries before this diagonal */
    idx_t within   = n - prev - 1;             /* 0‑based offset inside the diagonal */
    idx_t n_row    = within + 1;               /* row index (n in A(n,k))          */
    idx_t k_col    = s - n_row;                /* column index (k in A(n,k))       */

    seq_t sum = 0;

    /* Sum over all divisors of n_row */
    for (idx_t d = 1; d * d <= n_row; ++d)
    {
        if (n_row % d == 0)
        {
            seq_t exp1 = (seq_t)(n_row / d) + (seq_t)k_col;
            sum += big_pow((seq_t)d, exp1);

            idx_t d2 = n_row / d;
            if (d2 != d)
            {
                seq_t exp2 = (seq_t)(n_row / d2) + (seq_t)k_col; /* = d + k_col */
                sum += big_pow((seq_t)d2, exp2);
            }
        }
    }

    return sum;
}
