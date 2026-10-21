/*
 * A295687  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 2, a(2) =
 *          2, a(3) = 1.
 * https://oeis.org/A295687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295687
   a(0) = 1, a(1) = 2, a(2) = 2, a(3) = 1
   a(n) = a(n-1) + a(n-3) + a(n-4)   for n ≥ 4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise base terms on first call */
    if (cache == NULL) {
        cache_size = 4;                       /* indices 0..3 are valid */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;          /* out of memory – fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* if we already have the term, return it */
    if (n < cache_size)
        return cache[n];

    /* we need to grow the cache to hold index n   */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;  /* out of memory – fallback */
        cache = tmp;
        /* old cache_size is the index from which to start computing */
        idx_t start = cache_size;
        cache_size = new_size;

        /* compute all new terms up to n inclusive */
        for (idx_t i = start; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }

    return cache[n];
}
