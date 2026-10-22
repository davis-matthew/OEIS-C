/*
 * A307668  A(n,k) = Sum_{j=0..floor(n/k)} (-1)^j*binomial(2*n,k*j+n), square
 *          array A(n,k) read by antidiagonals, for n >= 0, k >= 1.
 * https://oeis.org/A307668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * A(i,k) =
 *            ∑_{j=0}^{⌊i/k⌋} (-1)^j * binomial(2*i, k*j + i) * along
 *            antidiagonals with k ≥ 1. For a given n we find the * unique
 *            pair (i,k) such that i + k = d and the element is the * n-th in
 *            the antidiagonal ordering. Then a(n) = A(i,k)
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d (starting from 1) that contains the n-th
     * element.  The cumulative number of elements up to diagonal d
     * is d*(d+1)/2.  Find minimal d with d*(d+1)/2 > n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 <= n)
        ++d;

    /* Position of the element within this diagonal (0‑based). */
    idx_t prev_sum = (d - 1) * d / 2;
    idx_t pos      = n - prev_sum;    /* 0 <= pos < d */

    idx_t i = pos;      /* row index */
    idx_t k = d - i;    /* column index >= 1 */

    /* Compute A(i,k) by its defining summation. */
    idx_t max_j = (k == 0) ? 0 : i / k;   /* floor(i/k) */
    seq_t sum = 0;

    for (idx_t j = 0; j <= max_j; ++j)
    {
        idx_t arg = k * j + i;          /* k*j + i */
        seq_t term = binomial(2 * i, arg);
        if (j & 1)
            sum -= term;                /* (-1)^j */
        else
            sum += term;
    }

    return sum;
}
