/*
 * A388052  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) = Sum_{j=0..n} binomial(n,j) * binomial(k*n+j+1,n).
 * https://oeis.org/A388052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = Σ_{j=0..i} binomial(i,j) * binomial(k*i + j + 1, i) *
 *            where i = n - T(d), k = d - i, * T(d) = d(d+1)/2 (triangular
 *            number), * d = floor((sqrt(8*n + 1) - 1)/2). * This gives the
 *            antidiagonal enumeration of the square array * A(n,k) defined by
 *            OEIS A388052
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that T(d) <= n < T(d+1) */
    idx_t d = (idx_t)((sqrtl((long double)8 * (long double)n + 1) - 1) / 2);

    /* Adjust d if rounding errors occur */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    /* Position within diagonal */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;          /* row index n */
    idx_t k = d - offset;      /* column index k */

    /* Compute A(i,k) = Σ_{j=0..i} binomial(i,j)*binomial(k*i + j + 1,i) */
    seq_t result = 0;
    for (idx_t j = 0; j <= i; ++j) {
        seq_t term = binomial(i, j) * binomial(k * i + j + 1, i);
        result += term;
    }

    return result;
}
