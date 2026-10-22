/*
 * A355324  Lower midsequence of the Fibonacci numbers (1,2,3,5,8,...) and
 *          Lucas numbers (1,3,4,7,11,...); see Comments.
 * https://oeis.org/A355324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5); Initial
 *            values: a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=9
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored in cache */
    static idx_t cache_cap  = 0;   /* allocated capacity of cache */

    /* Initial values for the first five terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6, (seq_t)9 };

    /* If the requested index is within the initial window, no need to
       touch dynamic memory. */
    if (n < 5)
        return init[(size_t)n];

    /* Ensure cache has enough allocated space for indices up to n. */
    if (cache_cap == 0) {
        cache_cap = 8;                      /* start with capacity 8 */
        cache = (seq_t*)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) { return (seq_t)0; }     /* allocation failure */
    }
    if (n + 1 > (idx_t)cache_cap) {
        /* Grow the cache geometrically until it can hold (n+1) entries. */
        idx_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) { return (seq_t)0; }       /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache entries up to at least index 5 (if not yet set). */
    if (cache_size < 5) {
        for (size_t i = cache_size; i < 5; ++i)
            cache[i] = init[i];
        cache_size = 5;
    }

    /* Compute values from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 5) {                       /* safety check */
            cache[i] = init[(size_t)i];
        } else {
            /* Apply the recurrence relation. */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                       - cache[i-4] - cache[i-5];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
