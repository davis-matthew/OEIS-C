/*
 * A393915  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k) * k!.
 * https://oeis.org/A393915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(2*k,k) * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum        = 1;    /* term for k = 0 */
    seq_t binom_nk   = 1;    /* C(n,0) */
    seq_t binom_2k_k = 1;    /* C(0,0) */
    seq_t factk      = 1;    /* 0! */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update C(n,k) = C(n,k-1) * (n-k+1) / k */
        binom_nk = binom_nk * ((seq_t)(n - k + 1)) / (seq_t)k;

        /* update C(2k,k) = C(2(k-1),k-1) * (2k)*(2k-1) / (k*k) */
        seq_t numerator   = (seq_t)(2 * k) * (seq_t)(2 * k - 1);
        seq_t denominator = (seq_t)k * (seq_t)k;
        binom_2k_k = binom_2k_k * numerator / denominator;

        /* update k!  */
        factk = factk * (seq_t)k;

        /* term = C(n,k)^2 * C(2k,k) * k! */
        seq_t term = binom_nk * binom_nk;
        term = term * binom_2k_k;
        term = term * factk;

        sum += term;
    }
    return sum;
}
