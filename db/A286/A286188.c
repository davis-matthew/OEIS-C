/*
 * A286188  Number of connected induced (non-null) subgraphs of the gear graph
 *          with 2n+1 nodes.
 * https://oeis.org/A286188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) + 3*a(n-4) - 2*a(n-5),
 * with initial values a(1)=6, a(2)=26, a(3)=76, a(4)=218, a(5)=664.
 */

#include <stdlib.h>
#include <string.h>          /* for memset */

seq_t a(idx_t n)
{
    /* Offset is 1: return 0 for n==0 (used by some tests). */
    if (n == 0) return (seq_t)0;

    /* Static state for memoisation. */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t  cache_cap = 0;         /* allocated size (index ≤ cache_cap) */
    static idx_t  computed_max = 0;      /* largest n for which cache[n] is valid */

    /* Helper to ensure the cache is large enough for indices up to 'need'. */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        /* Zero initialise the newly allocated region. */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms exactly once. */
    if (computed_max < 5) {
        if (cache_cap < 5) {
            idx_t new_cap = 5;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          (size_t)(new_cap + 1) * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
                cache[i] = (seq_t)0;
            cache_cap = new_cap;
        }
        if (computed_max < 1) cache[1] = (seq_t)6;
        if (computed_max < 2) cache[2] = (seq_t)26;
        if (computed_max < 3) cache[3] = (seq_t)76;
        if (computed_max < 4) cache[4] = (seq_t)218;
        if (computed_max < 5) cache[5] = (seq_t)664;
        computed_max = 5;
    }

    /* Compute any missing terms up to the requested n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = 6 * cache[i - 1]
                    - 10 * cache[i - 2]
                    + 4 * cache[i - 3]
                    + 3 * cache[i - 4]
                    - 2 * cache[i - 5];
        cache[i] = val;
    }
    if (n > computed_max) computed_max = n;

    return cache[n];
}
