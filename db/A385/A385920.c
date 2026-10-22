/*
 * A385920  E.g.f. A(x) satisfies A(x) = exp(x*A(x) + x^3*A''(x)).
 * https://oeis.org/A385920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} (1 + k³) · C(n-1,k) · a(k) · a(n-1-k)
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    if (n == 0)              /* a(0) = 1 */
        return (seq_t)1;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {            /* should not occur because n>0, but keep safe */
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum   = 0;
            seq_t binom = 1;
            idx_t m     = i - 1;
            for (idx_t k = 0; k <= m; ++k) {
                seq_t k_s   = (seq_t)k;
                seq_t k3    = k_s * k_s * k_s;          /* k³      */
                seq_t coeff = 1 + k3;                    /* 1 + k³  */
                seq_t term  = coeff * binom;            /* coeff * C(m,k) */
                term        = term * cache[k];           /* * a(k)   */
                term        = term * cache[m - k];       /* * a(m-k) */
                sum        += term;                      /* accumulate */
                if (k < m) {                            /* prepare next binomial */
                    binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
                }
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }
    return cache[n];
}
