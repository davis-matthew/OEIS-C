/*
 * A376568  Expansion of 1/(1 - 9*x*(1 + x))^(2/3).
 * https://oeis.org/A376568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376568
   Formula: a(n) = 3*((3*n-1)*a(n-1) + (3*n-2)*a(n-2))/n
   a(0) = 1, a(1) = 6 */
#include <stdlib.h>
#include <stddef.h>

/* Memoised recursive computation */
seq_t a(idx_t n)
{
    /* Base cases handled explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* Static cache for memoisation */
    static seq_t *cache     = NULL;  /* array of computed values   */
    static idx_t  cache_sz  = 0;    /* number of valid entries    */

    /* Initialise cache with the two known terms */
    if (cache_sz < 2) {
        cache_sz = 2;
        cache    = (seq_t *)realloc(cache, cache_sz * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    /* Return already computed value if present */
    if (n < cache_sz)
        return cache[n];

    /* Expand the cache to include n */
    idx_t new_sz = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (new_cache == NULL)
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* Compute missing terms up to n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t a_prev  = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];

        idx_t t1 = 3 * i - 1;  /* 3*i-1  */
        idx_t t2 = 3 * i - 2;  /* 3*i-2  */

#ifdef __SIZEOF_INT128__
        /* Use 128‑bit arithmetic when seq_t fits in ≤64 bits */
        if (sizeof(seq_t) <= 8) {
            __uint128_t num = (__uint128_t)((__uint128_t)t1 * a_prev
                                            + (__uint128_t)t2 * a_prev2);
            num *= 3;
            cache[i] = (seq_t)(num / i);
        } else
#endif
        {
            /* Pure seq_t arithmetic (handles arbitrary precision) */
            seq_t sum = (seq_t)t1 * a_prev + (seq_t)t2 * a_prev2;
            cache[i]  = sum * 3 / i;
        }
    }

    cache_sz = new_sz;
    return cache[n];
}
