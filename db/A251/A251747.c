/*
 * A251747  9-step Fibonacci sequence starting with 0,0,0,0,0,0,1,0,0.
 * https://oeis.org/A251747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+9) =
 *            a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7)+a(n+8)
 *            Initial values (AEIS offset 0):
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1,a(7)=0,a(8)=0
 *            Compute memoized on demand
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 41, u64 n <= 73, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 9 terms */
    static const seq_t init[9] = {0, 0, 0, 0, 0, 0, 1, 0, 0};

    /* Return directly if n is within the initial 9 terms */
    if (n < 9)
        return init[n];

    /* Static cache for memoisation */
    static seq_t *cache        = NULL;   /* array of computed terms */
    static idx_t  cache_size   = 0;      /* number of initialized entries in cache */
    static idx_t  cache_cap    = 0;      /* current capacity of cache array */

    /* Expand cache capacity to at least n+1 */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 9;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Ensure initial 9 terms are cached */
    if (cache_size < 9) {
        for (idx_t i = cache_size; i < 9; ++i)
            cache[i] = init[i];
        cache_size = 9;
    }

    /* If we already have enough values, return cached result */
    if (n < cache_size)
        return cache[n];

    /* Compute sum of the last 9 cached values */
    seq_t sum = 0;
    for (idx_t i = cache_size - 9; i < cache_size; ++i)
        sum += cache[i];

    /* Extend cache up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = sum;
        sum += cache[i] - cache[i - 9];   /* update running sum of last 9 terms */
    }
    cache_size = n + 1;

    return cache[n];
}
