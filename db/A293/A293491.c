/*
 * A293491  a(n) = n! * [x^n] exp((n+2)*x)*BesselI(0,2*x).
 * https://oeis.org/A293491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*binomial(2*k,k)*n^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Prepare constants */
    seq_t N = (seq_t)n;        /* base n */
    seq_t sum = (seq_t)0;     /* accumulator for the sum */

    /* Starting values for the iterative computation */
    seq_t pow = big_pow(N, (seq_t)n);   /* n^n */
    seq_t binom_nk = (seq_t)1;          /* binomial(n,0) */
    seq_t binom_2k_k = (seq_t)1;        /* binomial(0,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom_nk * binom_2k_k * pow */
        seq_t term = binom_nk * binom_2k_k;
        term = term * pow;
        sum += term;

        /* Stop early when the last term has been added */
        if (k == n)
            break;

        /* Compute binom(n, k+1) from binom(n, k) */
        binom_nk = binom_nk * (N - (seq_t)k);
        binom_nk = binom_nk / (seq_t)(k + 1);

        /* Compute binom(2(k+1), k+1) from binom(2k, k) */
        seq_t num1 = (seq_t)(2 * k + 1);
        seq_t num2 = (seq_t)(2 * k + 2);
        seq_t denom = (seq_t)(k + 1);
        binom_2k_k = binom_2k_k * num1 * num2;
        binom_2k_k = binom_2k_k / (denom * denom);

        /* Update n^(n-k-1) for the next iteration */
        pow = pow / N;
    }

    return sum;
}
