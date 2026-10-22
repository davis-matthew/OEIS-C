/*
 * A365778  Expansion of e.g.f. (exp(3*x) / (2 - exp(3*x)))^(1/6).
 * https://oeis.org/A365778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+∑_{k=1}^{n-1} 3^k·C(n-1,k)·a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, abort, size_t */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;   /* capacity of cache array */
    static idx_t   last      = 0;   /* index of the last computed term */

    /* ensure the cache is large enough for the needed index n */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        last = 0;
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n (sequence is memoised) */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {          /* a(0) is already 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        /* start with a(i-1) */
        seq_t sum = cache[i - 1];

        /* binomial coefficients C(i-1, k) for k=1..i-1 */
        seq_t binom  = (seq_t)1;          /* C(i-1,0) = 1 */
        seq_t pow3k  = (seq_t)3;          /* 3^k, start with k=1 */

        for (idx_t k = 1; k < i; ++k) {
            /* update binomial coefficient to C(i-1, k) */
            binom = (binom * (seq_t)(i - k)) / (seq_t)k;

            /* term = binom * 3^k * a(i-k) */
            seq_t term = binom;
            term = term * pow3k;
            term = term * cache[i - k];

            sum += term;

            /* prepare 3^(k+1) for the next iteration */
            pow3k *= (seq_t)3;
        }

        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
