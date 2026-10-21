/*
 * A299254  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 3^4.6 2D tiling (cf. A250120).
 * https://oeis.org/A299254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) for n>7
 *            initial values: a(0)=1, a(1)=7, a(2)=21, a(3)=45, a(4)=79,
 *            a(5)=122, a(6)=175, a(7)=237
 * Range:     u8 n <= 7, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* cached terms */
    static size_t cache_len = 0;    /* number of terms actually stored   */
    static size_t cache_cap = 0;    /* current allocator capacity        */

    /* initial values for indices 0..7 */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)7, (seq_t)21, (seq_t)45, (seq_t)79,
        (seq_t)122, (seq_t)175, (seq_t)237
    };

    /* enlarge storage if necessary */
    size_t need = (size_t)n + 1;            /* we need indices 0..n inclusive */
    if (need > cache_cap) {
        /* allocate new array */
        seq_t *new_cache = NULL;
#ifdef __cplusplus
        new_cache = new seq_t[need];
#else
        new_cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!new_cache) abort();
#endif
        /* copy old contents */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        /* free old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = need;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < need; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            cache[i] = 2 * cache[i - 1]
                     - cache[i - 2]
                     + cache[i - 5]
                     - 2 * cache[i - 6]
                     + cache[i - 7];
        }
    }

    cache_len = need;
    return cache[n];
}
