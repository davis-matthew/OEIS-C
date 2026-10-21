/*
 * A248226  a(n) = 10^n - 3^n.
 * https://oeis.org/A248226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n − 3^n Recurrence: a(0)=0, a(1)=7,
 *            a(n)=13·a(n−1)−30·a(n−2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Cache of previously computed values.
        cache[0..cache_len‑1] holds a(0) … a(cache_len‑1).  */
    static seq_t *cache   = NULL;   /* sequence array */
    static idx_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    /* Grow the cache if the requested index is beyond the current storage. */
    if (n >= cache_len) {
        /* Determine the new capacity (at least 16, doubled each time). */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise base cases if the cache is empty. */
        if (cache_len == 0) {
            cache[0] = 0;              /* a(0) */
            cache_len = 1;
        }
        if (cache_len == 1 && n >= 1) {
            cache[1] = 7;              /* a(1) */
            cache_len = 2;
        }

        /* Fill missing terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)13 * cache[i-1] - (seq_t)30 * cache[i-2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
