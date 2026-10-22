/*
 * A363867  a(n) = A108625(n,2*n).
 * https://oeis.org/A363867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} C(n,k)^2 · C(2 n+k, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* multiplicative computation of the triangular‑array binomials */
    seq_t sum = 0;
    seq_t binom_nk = 1;                                 /* C(n,0)  */
    seq_t binom_2nk = binomial((idx_t)(2 * n), n);      /* C(2n ,n) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k)^2 · C(2n+k,n) */
        seq_t t_sq = binom_nk * binom_nk;
        seq_t term = t_sq * binom_2nk;
        sum += term;

        /* prepare next k: C(n,k+1) and C(2n+k+1,n) */
        if (k < n) {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);
            /* C(2n+k+1,n) = C(2n+k,n) * (2n+k+1) / (n+k+1) */
            binom_2nk = binom_2nk * (seq_t)(2 * n + k + 1) /
                         (seq_t)(n + k + 1);
        }
    }

    return sum;
}
