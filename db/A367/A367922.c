/*
 * A367922  Expansion of e.g.f. 1/(1 - x + 2*log(1 - x)).
 * https://oeis.org/A367922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 2*Sum_{k=1..n} (k-1)! * binomial(n,k) *
 *            a(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;          /* number of allocated elements */
    static idx_t  last_computed = (idx_t)-1; /* highest index already computed */

    /* Allocate cache of size 1 if it does not exist yet */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)realloc(NULL, cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise new slots to zero – they will be overwritten */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* If we already have this value, just return it */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute all values from last_computed+1 up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binomial(cur,0) */
        seq_t fac   = (seq_t)1;   /* (k-1)! for k=1 */

        for (idx_t k = 1; k <= cur; ++k) {
            /* Update binomial coefficient to binomial(cur,k) */
            binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;
            /* Add contribution to the sum */
            sum += fac * binom * cache[cur - k];
            /* Prepare (k)! for next iteration */
            fac *= (seq_t)k;
        }

        cache[cur] = (seq_t)cur * cache[cur - 1] + (seq_t)2 * sum;
    }

    last_computed = n;
    return cache[n];
}
