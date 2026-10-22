/*
 * A350986  a(0)=3, a(1)=105, a(2)=3567; a(n) = 35*(a(n-1)-a(n-2))+a(n-3).
 * https://oeis.org/A350986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-1) - a(n-2); a(0) = 3, a(1) = 105. This
 *            implementation caches terms so that calls in any order run in
 *            time proportional to the largest n requested
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first two terms if needed */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* unlikely failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)105;
        cache_len = 2;
    }

    /* if n is already known, just return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache to hold term n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* unlikely failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i < needed; ++i) {
        /* recurrence holds for i >= 2; cache_len is at least 2 */
        cache[i] = (seq_t)(34) * cache[i - 1] - cache[i - 2];
    }
    cache_len = needed;

    return cache[(size_t)n];
}
