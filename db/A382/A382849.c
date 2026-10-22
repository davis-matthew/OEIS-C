/*
 * A382849  a(n) = Sum_{k=0..n} (-1)^(n-k) * (binomial(n,k) *
 *          binomial(n+k,k))^2.
 * https://oeis.org/A382849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (binomial(n,k) *
 *            binomial(n+k,k))^2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Results for the accumulation */
    seq_t result = 0;

    /* Binomial coefficients that can be updated iteratively */
    seq_t binom1 = 1;  /* binomial(n,0) */
    seq_t binom2 = 1;  /* binomial(n+0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute (binom1 * binom2)^2 */
        seq_t prod = binom1 * binom2;
        prod = prod * prod;

        /* Add or subtract according to (-1)^(n-k) */
        if ((n - k) & 1) {
            result -= prod;
        } else {
            result += prod;
        }

        /* Prepare next binomial values, but skip when k reaches n */
        if (k == n)
            break;

        /* binom(n, k+1) = binom(n, k) * (n-k)/(k+1) */
        binom1 = binom1 * (seq_t)(n - k) / (seq_t)(k + 1);

        /* binom(n+k+1, k+1) = binom(n+k, k) * (n+k+1)/(k+1) */
        binom2 = binom2 * (seq_t)(n + k + 1) / (seq_t)(k + 1);
    }

    return result;
}
