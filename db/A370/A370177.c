/*
 * A370177  a(n) = floor(x*a(n-1)) for n > 0 where x = (7 + sqrt(77))/2, a(0)
 *          = 1.
 * https://oeis.org/A370177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 7*a(n-3) for n > 2 with initial values a(0)=1,
 *            a(1)=7, a(2)=55
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; grows on demand */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* number of cache slots currently allocated */

    /* we need indices 0..n, so ensure we have n+1 slots */
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        /* grow the cache */
        seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed: return 0 as error code */
        cache = new_cache;
        size_t old_size = cache_size;
        cache_size = needed;

        /* initialise base values if they were not already set */
        if (old_size <= 0 && cache_size > 0) cache[0] = (seq_t)1;
        if (old_size <= 1 && cache_size > 1) cache[1] = (seq_t)7;
        if (old_size <= 2 && cache_size > 2) cache[2] = (seq_t)55;

        /* compute any new terms using the 3‑term recurrence */
        size_t start = (old_size > 3) ? old_size : 3;
        for (size_t i = start; i < cache_size; ++i) {
            /* a(i) = 8*a(i-1) - 7*a(i-3) */
            cache[i] = (seq_t)8 * cache[i-1] - (seq_t)7 * cache[i-3];
        }
    }

    return cache[(size_t)n];
}
