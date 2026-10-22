/*
 * A395783  Expansion of (Sum_{k>=0} binomial(3*k+2,k) * x^k)^3.
 * https://oeis.org/A395783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(3*n+8, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t pow2  = 1;
    idx_t N = 3 * n + (idx_t)8;
    seq_t binom = binomial(N, n);

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = (seq_t)(k + 1) * pow2 * binom;
        sum += term;

        if (k == n) break;

        pow2 *= 2;

        /* Update binom for next k:
         * binom_{k+1} = binom_k * (n - k) / (2*n + k + 9)   */
        seq_t num   = binom * (seq_t)(n - k);
        seq_t denom = (seq_t)(2 * n + k + 9);
        binom = num / denom;
    }

    return sum;
}
