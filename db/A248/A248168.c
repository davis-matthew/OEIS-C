/*
 * A248168  Expansion of g.f. 1/sqrt((1-3*x)*(1-11*x)).
 * https://oeis.org/A248168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * 2^k * C(n,k) * C(2k,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the central coefficient of (1+7*x+4*x^2)^n for n >= 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* current binomial C(n,k) */
    seq_t binom_n_k = 1;
    /* current central binomial C(2k,k) */
    seq_t binom_2k_k = 1;
    /* powers of 2 and 3 */
    seq_t pow2_k   = 1;   /* 2^k, starts at k=0 */
    seq_t pow3_nk  = 1;   /* 3^(n-k), starts at k=0 */

    /* compute 3^n for the first iteration (k = 0) */
    for (idx_t i = 0; i < n; ++i)
        pow3_nk *= 3;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 3^(n-k) * 2^k * C(n,k) * C(2k,k) */
        seq_t term = binom_n_k;
        term *= binom_2k_k;
        term *= pow2_k;
        term *= pow3_nk;
        sum += term;

        if (k == n) /* last iteration, no further updates needed */
            break;

        /* update C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        seq_t num_n = (seq_t)(n - k);
        seq_t den_n = (seq_t)(k + 1);
        binom_n_k = (binom_n_k * num_n) / den_n;

        /* update C(2(k+1),k+1) = C(2k,k)*(2k+2)*(2k+1)/((k+1)^2) */
        seq_t tmp  = (seq_t)k;
        seq_t num2 = (tmp * 2 + 2) * (tmp * 2 + 1);
        seq_t den2 = (tmp + 1) * (tmp + 1);
        binom_2k_k = (binom_2k_k * num2) / den2;

        /* update powers for next k */
        pow2_k  *= 2;
        pow3_nk /= 3;
    }

    return sum;
}
