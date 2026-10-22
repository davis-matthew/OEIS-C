/*
 * A385319  a(n) = Sum_{k=0..n} 2^k * binomial(2*n,k) * binomial(2*n-k-1,n-k).
 * https://oeis.org/A385319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(2*n,k) *
 *            binomial(2*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const idx_t two_n = 2 * n;

    seq_t sum   = 0;
    seq_t pow2  = 1;                    /* 2^k  */
    seq_t binom1 = 1;                   /* binom(2n, 0) */
    seq_t binom2 = binomial(two_n - 1, n); /* binom(2n-1, n) */

    /* k = 0 term */
    sum += pow2 * binom1 * binom2;

    for (idx_t k = 0; k < n; ++k) {
        /* advance to next k = k+1 */
        pow2   = pow2 * (seq_t)2;
        binom1 = binom1 * ((seq_t)(two_n - k)) / ((seq_t)(k + 1));
        binom2 = binom2 * ((seq_t)(n - k))     / ((seq_t)(two_n - k - 1));

        sum += pow2 * binom1 * binom2;
    }

    return sum;
}
