/*
 * A356124  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} j^k *
 *          binomial(floor(n/j)+1,2).
 * https://oeis.org/A356124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) where (n,k) is given by the antidiagonal reading
 *            downwards of the square array T(n,k) = Sum_{j=1..n} j^k *
 *            binomial(floor(n/j)+1,2)
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t N)
{
    if (N == 0) return 0;

    /* Find diagonal d such that 1 + d*(d+3)/2 >= N  */
    idx_t d = 0;
    while ((unsigned __int128)1 +
           (unsigned __int128)d * (d + 3) / 2 < N)
        ++d;

    /* Number of terms before this diagonal */
    idx_t cum_prev;
    if (d == 0)
        cum_prev = 0;
    else
        cum_prev = (idx_t)(1 +
                           (unsigned __int128)(d - 1) * (d + 2) / 2);

    /* Position within diagonal (0‑based) */
    idx_t pos = N - cum_prev - 1;

    /* Corresponding (n,k) */
    idx_t n = pos + 1;
    idx_t k = d - pos;

    seq_t result = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jpow = pow_uint(j, k);
        idx_t t = n / j;
        seq_t x = (seq_t)(t + 1);
        seq_t bin = x * (x - 1) / 2;
        result += jpow * bin;
    }
    return result;
}
