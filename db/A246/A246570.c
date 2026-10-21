/*
 * A246570  G.f.: Sum_{n>=0} x^n / (1-x)^(4*n+1) * [Sum_{k=0..2*n} C(2*n,k)^2
 *          * x^k]^2.
 * https://oeis.org/A246570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..k} C(2*n - k - j, k)^2 * C(k,j)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* initial m for j = 0 is 2*n - k */
        idx_t m = 2 * n - k;
        seq_t binom_val = binomial(m, k);   /* C(m, k) */
        seq_t curCk = 1;                    /* C(k, 0) */
        idx_t m_curr = m;                   /* current m value */

        for (idx_t j = 0; j <= k; ++j) {
            seq_t tmp = binom_val * curCk;  /* product */
            tmp = tmp * tmp;               /* square */
            result += tmp;

            if (j == k) break;             /* last iteration, no update */

            /* update C(k, j+1) = C(k, j) * (k-j) / (j+1) */
            seq_t tmp1 = curCk * (seq_t)(k - j);
            curCk = tmp1 / (seq_t)(j + 1);

            /* update C(m-j-1, k) = C(m-j, k) * (m-j-k) / (m-j) */
            seq_t tmp2 = binom_val * ( seq_t)(m_curr - k );
            binom_val = tmp2 / (seq_t)(m_curr);
            --m_curr;
        }
    }
    return result;
}
