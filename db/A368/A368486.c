/*
 * A368486  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^j * j^k.
 * https://oeis.org/A368486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the value of the square array T(n,k) read by
 *            antidiagonals downwards. For T(n,k) we use the definition T(n,k)
 *            = Σ_{j=0..n} k^j * j^k. The sequence a is obtained by listing
 *            the array elements in the order: for each diagonal d (where d =
 *            n + k), (0,d), (1,d-1), … , (d,0) (top to bottom). Th...
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_pow_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow_small.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2.
       Compute d incrementally to avoid overflow. */
    idx_t d   = (idx_t)0;
    idx_t cum = (idx_t)0;          /* sum of lengths of previous diagonals */
    while (cum + d + (idx_t)1 <= n)
    {
        cum += d + (idx_t)1;
        ++d;
    }

    /* Position along the diagonal (0 = top element). */
    idx_t pos = n - cum;           /* 0 <= pos <= d */
    idx_t i   = pos;               /* row index */
    idx_t j   = d - pos;           /* column index */

    /* Compute T(i,j) = Σ_{t=0..i} j^t * t^j. */
    seq_t sum = (seq_t)0;
    for (idx_t t = (idx_t)0; t <= i; ++t)
    {
        seq_t j_pow_t = seq_pow_small((seq_t)j, t);
        seq_t t_pow_j = seq_pow_small((seq_t)t, j);
        sum += j_pow_t * t_pow_j;
    }

    return sum;
}
