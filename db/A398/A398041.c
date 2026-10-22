/*
 * A398041  G.f. A(x) satisfies A(x) = A(x^3) / (1-2*x), with A(0) = 1.
 * https://oeis.org/A398041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1 if 3|n then a(n)=2*a(n-1)+a(n/3) else
 *            a(n)=2*a(n-1). Computes with memoized dynamic programming
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of computed entries */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* initialise cache if first call */
    if (!cache) {
        cache_cap = 1024;  /* start with a reasonable size */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = calloc(cache_cap, sizeof(seq_t));
#endif
        /* base case */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* grow cache if needed */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        /* copy existing values */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        cache = realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
#endif
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i % 3 == 0)
            cache[i] = cache[i - 1] * 2 + cache[i / 3];
        else
            cache[i] = cache[i - 1] * 2;
    }
    cache_size = (size_t)n + 1;

    return cache[n];
}
