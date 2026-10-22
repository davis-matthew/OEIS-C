/*
 * A301683  Partial sums of A301682.
 * https://oeis.org/A301683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) Initial
 *            terms: a(0) = 1, a(1) = 7, a(2) = 13, a(3) = 31, a(4) = 49, a(5)
 *            = 67, a(6) = 103
 * Range:     u8 n <= 9, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t  cache_len = 0;   /* number of values actually stored */

    /* Ensure cache has at least the first 7 terms initialized. */
    if (cache_cap < 7) {
        size_t new_cap = 7;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation error – should not happen */
        cache = new_cache;
        cache_cap = new_cap;

        /* initialise the first 7 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)49;
        cache[5] = (seq_t)67;
        cache[6] = (seq_t)103;

        cache_len = 7;
    }

    /* If the requested index is already cached, return immediately. */
    if (n < cache_len)
        return cache[n];

    /* Expand cache if needed.    */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation error – should not happen */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute from cache_len up to n inclusive.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute using a safe order to avoid unsigned underflow:
           sum of positive terms first, then subtract the sum of negative terms.     */
        seq_t part1 = cache[i - 1] + (cache[i - 3] << 1) + cache[i - 7];
        seq_t part2 = (cache[i - 4] << 1) + cache[i - 6];
        cache[i] = part1 - part2;
    }

    cache_len = n + 1;   /* we've filled up to n */
    return cache[n];
}
