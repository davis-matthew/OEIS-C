/*
 * A274635  Square array read by antidiagonals: T(m,n) (m>=0, n>=0) =
 *          (2^floor(m/2)-1)*(2^ceiling(n/2)-1).
 * https://oeis.org/A274635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^⌊m/2⌋ - 1) * (2^⌈n/2⌉ - 1) for the cell (m,n) of the
 *            square array, * read by antidiagonals with each diagonal
 *            traversed from right to left. * That is, for an index n (offset
 *            0) we compute the diagonal k and the position * within that
 *            diagonal, derive m and n from them, and evaluate the...
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find diagonal k such that the cumulative number of elements up to k-1
       is <= n < cumulative up to k. The cumulative number of elements up to
       diagonal k is (k+1)(k+2)/2. */
    idx_t k  = 0;
    idx_t cum = 0;          /* cumulative elements up to diagonal k-1 */

    while (1)
    {
        /* number of elements in diagonal k is k+1 */
        idx_t next = cum + (k + 1);
        if (next > n)
            break;
        cum = next;
        ++k;
    }

    /* Position within diagonal k */
    idx_t pos = n - cum;

    /* Coordinates of the cell: for the chosen traversal order,
       m decreases from k down to 0; n increases from 0 up to k. */
    idx_t m        = k - pos;   /* row index */
    idx_t n_coord  = pos;       /* column index */

    /* Compute the two powers of two */
    idx_t exp_f = m >> 1;                              /* floor(m/2) */
    idx_t exp_g = (n_coord + 1) >> 1;                  /* ceil(n/2) */

    seq_t f = pow2(exp_f);
    seq_t g = pow2(exp_g);

    /* Subtract 1 from each */
    f = f - (seq_t)1;
    g = g - (seq_t)1;

    /* Return the product */
    return f * g;
}
