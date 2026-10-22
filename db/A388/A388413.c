/*
 * A388413  a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+3,k).
 * https://oeis.org/A388413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n,k) * binomial(n+3,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 4^n once, then divide by 4 in each step. */
    seq_t power4 = pow_uint(4, n);

    /* Binomial coefficients C(n,k) and C(n+3,k) initialized at k = 0. */
    seq_t binom_n   = 1;      /* C(n,0) */
    seq_t binom_n3  = 1;      /* C(n+3,0) */

    /* Term for k = 0. */
    seq_t term = power4 * binom_n * binom_n3;

    seq_t sum = term;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update 4^(n-k) for the current k. */
        power4 = power4 / 4;

        /* Update binomial coefficients using multiplicative recurrence. */
        binom_n  = (binom_n * (seq_t)(n - k + 1)) / (seq_t)k;
        binom_n3 = (binom_n3 * (seq_t)(n + 3 - k + 1)) / (seq_t)k;

        /* Compute term and add to sum. */
        term = power4 * binom_n * binom_n3;
        sum += term;
    }

    return sum;
}
