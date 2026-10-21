/*
 * A297972  Number of n X 2 0..1 arrays with every element equal to 2, 3, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 2*a(n-3) for n ≥ 4, a(1) = 0, a(2) =
 *            1, a(3) = 3
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed values: cache[i] = a(i+1)  (0‑based). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;         /* number of entries stored */

    if (n < 1)                          /* offset is 1, below 1 is undefined */
        return 0;

    /* Initialize cache with the first three terms if not already done. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)  /* calloc failure – give up */
            return 0;
        cache[0] = 0;         /* a(1) */
        cache[1] = 1;         /* a(2) */
        cache[2] = 3;         /* a(3) */
    }

    /* Extend the cache up to the requested index if necessary. */
    if (n > cache_size) {
        idx_t old_size = cache_size;
        cache_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!new_cache)   /* realloc failure – give up */
            return 0;
        cache = new_cache;

        /* Compute missing terms.  Term n is stored at cache[n‑1]. */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            /* i >= 4 here, because old_size is at least 3. */
            seq_t val = cache[i-2] + (seq_t)3 * cache[i-3] + (seq_t)2 * cache[i-4];
            cache[i-1] = val;
        }
    }

    return cache[n-1];
}
