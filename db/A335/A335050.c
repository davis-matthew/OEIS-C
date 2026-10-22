/*
 * A335050  Array read by descending antidiagonals, T(n,k) is the number of
 *          nodes in the pill tree with initial conditions (n,k), for n and k
 *          >= 0.
 * https://oeis.org/A335050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i = offset, j = d - offset and T(i,j) =
 *            Σ_{t=0..i} ( binom(2t+j+2, t+1) - binom(2t+j+2, t) ) The pair
 *            (i,j) is obtained from the anti‑diagonal decomposition of n
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (1) {
        seq_t tri = ((seq_t)d * ((seq_t)d + 1)) / 2;
        if (tri > (seq_t)n) break;
        ++d;
    }
    --d;                              /* largest d with triangular <= n */
    seq_t tri_d = ((seq_t)d * ((seq_t)d + 1)) / 2;
    idx_t offset = n - (idx_t)tri_d;   /* position inside diagonal */

    idx_t i = offset;                 /* row index   */
    idx_t j = d - offset;             /* column index */

    seq_t sum = 0;
    for (idx_t t = 0; t <= i; ++t) {
        idx_t inner = 2 * t + j + 2;
        seq_t c1 = binom(inner, t + 1);
        seq_t c0 = binom(inner, t);
        sum += c1 - c0;
    }
    return sum;
}
