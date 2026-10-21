/*
 * A287837  Number of words over the alphabet {0,1,...,10} such that no two
 *          consecutive terms have distance 7.
 * https://oeis.org/A287837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 3*a(n-2), a(0)=1, a(1)=11, a(2)=113
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_max = 0;   /* largest index currently stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache array */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* emalloc-failure is fatal */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)113;
        cache_max = 2;
    }

    /* if requested index already cached, return it */
    if (n <= (idx_t)cache_max)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)10 * cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    cache_max = n;
    return cache[n];
}
