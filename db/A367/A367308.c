/*
 * A367308  a(0) = 1; thereafter a(n) = n*Sum_{k=0..n-1} binomial(n,k) * a(k).
 * https://oeis.org/A367308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * Σ_{k=0}^{n-1} binomial(n,k) * a(k)   (a(0) = 1)
 * Computes the terms of OEIS sequence A367308.
 * The function memoises previously computed values so calls may
 * come in any order.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;   /* cache[0..max] holds a(k) */
    static size_t cache_cap  = 0;      /* allocated array size     */
    static idx_t  cache_max  = 0;      /* largest n with value    */

    /* initialise on first call: a(0) = 1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache_max = 0;
        cache     = (seq_t *)malloc(sizeof(*cache) * cache_cap);
        if (cache == NULL) return 0;       /* allocation failed */
        cache[0] = 1;
    }

    /* if the requested term is already known, return it */
    if (n <= cache_max)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(*cache) * new_cap);
        if (tmp == NULL) return 0;      /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum   = 0;    /* Σ_{k=0}^{m-1} binomial(m,k) * a(k) */
        seq_t binom = 1;    /* binomial(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            sum += binom * cache[k];

            /* update binomial coefficient: binom = binom(m,k+1) */
            seq_t num = (seq_t)(m - k);
            seq_t den = (seq_t)(k + 1);
            binom = (binom * num) / den;
        }

        cache[m] = ((seq_t)m) * sum;
    }

    cache_max = n;
    return cache[n];
}
