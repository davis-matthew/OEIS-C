/*
 * A337388  a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) * binomial(2*n,2*k).
 * https://oeis.org/A337388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^(n-k) * binomial(2*k,k) *
 *            binomial(2*n,2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    /* Compute the initial power n^n (define 0^0 = 1) */
    seq_t pow_val = 1;
    if (n > 0) {
        pow_val = big_pow(n_seq, n_seq);
    }

    seq_t binom_2k_k = 1;   /* binomial(2*k, k) */
    seq_t binom_2n_2k = 1;  /* binomial(2*n, 2*k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = n^(n-k) * binom(2*k,k) * binom(2*n,2*k) */
        seq_t term = pow_val;
        term *= binom_2k_k;
        term *= binom_2n_2k;
        sum += term;

        if (k < n) {  /* Prepare for next k */
            /* Decrease exponent of n by 1 */
            pow_val = pow_val / n_seq;

            /* Update binom(2*k,k) */
            seq_t num_k = binom_2k_k * (seq_t)(2*k+2) * (seq_t)(2*k+1);
            seq_t den_k = (seq_t)(k+1) * (seq_t)(k+1);
            binom_2k_k = num_k / den_k;

            /* Update binom(2*n,2*k) */
            idx_t term1 = 2*n - 2*k;        /* 2*n - 2*k          */
            idx_t term2 = term1 - 1;        /* 2*n - 2*k - 1      */
            seq_t num_n = binom_2n_2k * (seq_t)term1 * (seq_t)term2;
            seq_t den_n = (seq_t)(2*k+2) * (seq_t)(2*k+1);
            binom_2n_2k = num_n / den_n;
        }
    }
    return sum;
}
