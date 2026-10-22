/*
 * A346183  a(n) = Sum_{k=0..n} binomial(n,k) * binomial((k+1)^2, n).
 * https://oeis.org/A346183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * binomial((k+1)^2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_n_k = 1;            /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* upper binomial argument (k+1)^2 */
        idx_t m = (k + 1) * (k + 1);
        seq_t binom_m_n = binom(m, n);
        sum += binom_n_k * binom_m_n;

        if (k < n) {
            /* compute next binomial(n,k+1) from current one */
            /* binomial(n,k+1) = binomial(n,k) * (n-k) / (k+1) */
            binom_n_k = binom_n_k * (n - k) / (k + 1);
        }
    }
    return sum;
}
