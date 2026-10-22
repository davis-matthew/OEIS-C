/*
 * A382753  Expansion of e.g.f. 3/(5 - 2*exp(3*x)).
 * https://oeis.org/A382753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=1..n} (3^(k-1) * binomial(n,k) * a(n-k)),
 *            a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of pre‑computed values */

    if (n + 1 > cache_cap) {
        /* grow cache to hold terms up to n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failed */
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[0] = 1;              /* base value */
                continue;
            }

            seq_t sum = 0;                 /* accumulator for the sum */
            seq_t binom = (seq_t)i;        /* binomial(i,1) */
            seq_t pow3  = 1;               /* 3^(k-1) starts with k=1 */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = 3^(k-1) * binomial(i,k) * a(i-k) */
                seq_t term = pow3 * binom * cache[i - k];
                sum += term;

                /* prepare next binomial coefficient and power of 3 */
                pow3 *= 3;
                if (k < i) {
                    seq_t numer = (seq_t)(i - k);
                    seq_t denom = (seq_t)(k + 1);
                    binom = binom * numer / denom;   /* exact division */
                }
            }
            cache[i] = sum * 2;                /* final factor 2 */
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
