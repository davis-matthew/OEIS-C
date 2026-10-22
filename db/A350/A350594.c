/*
 * A350594  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) is Sum_{j=0..2*n} (-1)^(n+j) * binomial(2*n,j)^k.
 * https://oeis.org/A350594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Σ_{j=0}^{2i} (-1)^{i+j} * binomial(2i,j)^k, where (i,k)
 *            is the coordinate on the antidiagonal determined by n via n = i
 *            + k + (i+k)(i+k+1)/2
 * Range:     i8 n <= 18, i16 n <= 30, i32 n <= 58, i64 n <= 96, i128 n <= 177,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute the antidiagonal index d such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2   */
    idx_t d = (idx_t)((sqrt((double)8 * (double)n + 1) - 1) / 2);
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    /* Convert index n into coordinates (i,k) within the diagonal. */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;
    idx_t k = d - offset;

    /* Compute T(i,k) = Σ_{j=0}^{2i} (-1)^{i+j} * binomial(2i,j)^k. */
    seq_t result = 0;
    idx_t two_i = 2 * i;
    for (idx_t j = 0; j <= two_i; ++j) {
        seq_t bin = binomial(two_i, j);
        seq_t pow = pow_seq(bin, (int)k);
        if (((i + j) & 1) == 0)
            result += pow;
        else
            result -= pow;
    }
    return result;
}
