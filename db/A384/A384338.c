/*
 * A384338  a(n) = Sum_{k=0..n} 2^k * binomial(3*n+1,k) * binomial(3*n-k,n-k).
 * https://oeis.org/A384338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^k * binomial(3*n+1,k) * binomial(3*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow2 = 1;                     /* 2^k, starting with k = 0 */
    seq_t binom1 = 1;                   /* binomial(3*n+1, 0) */
    seq_t binom2 = binomial(3 * n, n);  /* binomial(3*n, n)   */

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow2 * binom1 * binom2;

        if (k == n) break;             /* no further terms needed */

        /* update binom1 to binomial(3*n+1, k+1) */
        seq_t num1 = (seq_t)(3 * n + 1 - k);
        seq_t den1 = (seq_t)(k + 1);
        binom1 = binom1 * num1 / den1;

        /* update binom2 to binomial(3*n-(k+1), n-(k+1)) */
        seq_t num2 = (seq_t)(n - k);
        seq_t den2 = (seq_t)(3 * n - k);
        binom2 = binom2 * num2 / den2;

        /* multiply pow2 by 2 for next k */
        pow2 = mul_small(pow2, 2);
    }

    return sum;
}
