/*
 * A322245  a(n) = A322244(n)^2, the square of the central coefficient in (1 +
 *          3*x + 16x^2)^n.
 * https://oeis.org/A322245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A322244(n)^2, where A322244(n) = Sum_{k=0..n} 11^(n-k) *
 *            (-4)^k * binomial(n,k) * binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Precompute 11^n */
    seq_t pow11_n = seq_pow((seq_t)11, n);

    /* Variables for the loop */
    seq_t pow11     = pow11_n;    /* 11^(n-k) */
    seq_t pow4      = (seq_t)1;   /* 4^k (absolute value of (-4)^k) */
    seq_t binom_nk  = (seq_t)1;   /* binomial(n,k) */
    seq_t binom_2k_k = (seq_t)1;  /* binomial(2*k,k) */

    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the positive term: 11^(n-k) * 4^k * binomial(n,k) * binomial(2k,k) */
        seq_t term = pow11;
        term *= binom_nk;
        term *= binom_2k_k;
        term *= pow4;

        /* Add to even/odd sum */
        if ((k & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;

        /* Prepare for next k (except after the last iteration) */
        if (k == n)
            break;

        /* Update 11^(n-k-1) */
        pow11 /= (seq_t)11;

        /* Update 4^k -> 4^(k+1) */
        pow4 *= (seq_t)4;

        /* Update binomial(n, k+1) = binom_nk * (n-k) / (k+1) */
        binom_nk *= (seq_t)(n - k);
        binom_nk /= (seq_t)(k + 1);

        /* Update binomial(2k, k) for next k:
           binom(2(k+1), k+1) = binom(2k, k) * (4k+2) / (k+1) */
        binom_2k_k *= (seq_t)(4 * k + 2);
        binom_2k_k /= (seq_t)(k + 1);
    }

    /* The central coefficient A322244(n) = sum_even - sum_odd (always positive) */
    seq_t central = sum_even;
    central -= sum_odd;

    /* Return its square */
    return central * central;
}
