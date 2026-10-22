/*
 * A363870  a(n) = A108625(n, 3*n).
 * https://oeis.org/A363870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n, k)^2 * binomial(3*n + k, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Preparations */
    seq_t N      = (seq_t)n;
    seq_t twoN   = N * 2;
    seq_t threeN = N * 3;

    /* Current values of C(n,k) and B(3n+k, n) */
    seq_t Ck = 1;                                  /* binom(n,0)   */
    seq_t Bk = binomial((idx_t)threeN, (idx_t)n);   /* binom(3n, n) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Accumulate the term C(n,k)^2 * binom(3n+k, n) */
        sum += Ck * Ck * Bk;

        if (k < n) {
            /* Update C(n,k+1) = C(n,k) * (n - k) / (k + 1) */
            Ck = (Ck * (N - (seq_t)k)) / ((seq_t)(k + 1));

            /* Update B(3n+k+1, n) = B(3n+k, n) * (3n + k + 1) / (2n + k + 1) */
            seq_t num = threeN + (seq_t)k + 1;
            seq_t den = twoN   + (seq_t)k + 1;
            Bk = (Bk * num) / den;
        }
    }

    return sum;
}
