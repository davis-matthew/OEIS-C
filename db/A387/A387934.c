/*
 * A387934  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) = Sum_{j=0..n} binomial(n,j) * binomial(k*n+j,j).
 * https://oeis.org/A387934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..i} binom(i,j) * binom(k*i+j,j) where (i,k) are
 *            obtained by reading the square array A(i,k) by antidiagonals.
 *            This implements OEIS A387934
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2. */
    idx_t t = 0;
    while ((__uint128_t)(t + 1) * (t + 2) / 2 <= n)
        ++t;

    /* Offset within this diagonal. */
    idx_t offset = n - (idx_t)((__uint128_t)t * (t + 1) / 2);
    idx_t row = offset;          /* i */
    idx_t col = t - offset;      /* k */

    seq_t sum = 0;
    for (idx_t j = 0; j <= row; ++j)
    {
        seq_t bj = binom(row, j);
        seq_t bk = binom((idx_t)((__uint128_t)col * row + j), j);
        sum += bj * bk;
    }
    return sum;
}
