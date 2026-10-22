/*
 * A306684  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 2/(1 - x + sqrt(1 - 2*x +
 *          (1-4*k)*x^2)).
 * https://oeis.org/A306684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are the antidiagonal coordinates of n.
 *            A(i,j) = Sum_{t=0..⌊i/2⌋} j^t * binomial(i,t) *
 *            binomial(i-t,t)/(t+1) (OEIS A306684)
 * Range:     u8 n <= 50, u16 n <= 100, u32 n <= 245, u64 n <= 621,
 *            u128 n <= 1755, big any n
 * Helpers:   antidiagonal_coords, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/binomial.h"

static seq_t compute_A(idx_t n, idx_t k)
{
    seq_t result = (seq_t)0;
    idx_t max_t = n / 2;
    seq_t pow_k = (seq_t)1;           /* k^t   */
    for (idx_t t = 0; t <= max_t; ++t) {
        seq_t bin1 = binomial(n, t);
        seq_t bin2 = binomial(n - t, t);

        seq_t term = bin1;
        term *= bin2;                 /* bin1 * bin2 */
        term /= (seq_t)(t + 1);        /* divide by (t+1) */
        term *= pow_k;                /* multiply by k^t */

        result += term;

        /* update pow_k for next t */
        if (t < max_t)
            pow_k *= (seq_t)k;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return compute_A(i, j);
}
