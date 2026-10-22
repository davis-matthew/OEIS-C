/*
 * A381199  a(n) = (4*n)!/((n!)^2*(2*n)!)*Sum_{k=0..n}
 *          binomial(n,k)^2*binomial(2*k,k).
 * https://oeis.org/A381199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)!/((n!)^2*(2*n)!)*\n * Sum_{k=0..n}
 *            binomial(n,k)^2*binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* ----- compute the factor (4n)! / ((n!)^2*(2n)!) ----- */
    seq_t factor = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num  = (seq_t)(2 * n + i) * (seq_t)(3 * n + i);
        seq_t den  = (seq_t)i * (seq_t)i;
        /* exact division */
        factor = (factor * num) / den;
    }

    /* ----- compute the sum Σ_{k=0..n} binom(n,k)^2 * binom(2k,k) ----- */
    seq_t sum      = (seq_t)0;
    seq_t bin_n_k  = (seq_t)1;   /* binom(n,0)   */
    seq_t bin_2k_k = (seq_t)1;   /* binom(0,0)   */

    /* k = 0 term */
    sum += bin_n_k * bin_n_k * bin_2k_k;

    for (idx_t k = 1; k <= n; ++k) {
        /* binom(n,k) from binom(n,k-1) */
        bin_n_k = (bin_n_k * (n - k + 1)) / k;

        /* binom(2k,k) from binom(2(k-1),k-1) */
        seq_t two_k   = (seq_t)(2 * k);
        seq_t num2k   = two_k * (two_k - (seq_t)1);  /* (2k)*(2k-1) */
        seq_t den2k   = (seq_t)k * (seq_t)k;         /* k*k         */
        bin_2k_k = (bin_2k_k * num2k) / den2k;

        /* add the term */
        seq_t term = bin_n_k * bin_n_k;
        term     = term * bin_2k_k;
        sum += term;
    }

    /* ----- final result ----- */
    return factor * sum;
}
