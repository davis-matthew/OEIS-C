/*
 * A346184  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(k^2, n).
 * https://oeis.org/A346184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(k^2, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_nk = 1;                 /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(k^2, n) */
        seq_t binom_k2_n = 0;
        idx_t sq = k * k;                /* k^2 fits in 64‑bit for n <= 10000 */

        if (sq >= n) {
            binom_k2_n = 1;
            for (idx_t i = 1; i <= n; ++i) {
                seq_t top = (seq_t)(sq - i + 1);
                binom_k2_n = (binom_k2_n * top) / (seq_t)i;
            }
        }

        /* accumulate term */
        sum += binom_nk * binom_k2_n;

        /* update binomial(n,k+1) for next iteration */
        if (k < n) {
            binom_nk = (binom_nk * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }

    return sum;
}
