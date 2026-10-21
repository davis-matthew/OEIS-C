/*
 * A299283  Coordination sequence for "svh" 3D uniform tiling.
 * https://oeis.org/A299283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6), a(0)=1,
 *            a(1)=7, a(2)=22, a(3)=48, a(4)=84, a(5)=130, a(6)=186
 * Range:     u8 n <= 7, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maintains a memoised cache of computed values. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache has capacity for indices 0 .. n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failure */
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        /* zero any newly allocated slots */
        for (size_t i = cache_len; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        static const seq_t init[7] = {1, 7, 22, 48, 84, 130, 186};
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 6) {
                cache[i] = init[i];
            } else {
                cache[i] = 2 * cache[i-1]            /* 2*a(n-1) */
                         - cache[i-2]                 /* - a(n-2) */
                         + cache[i-4]                 /* + a(n-4) */
                         - 2 * cache[i-5]             /* - 2*a(n-5) */
                         + cache[i-6];                /* + a(n-6) */
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
