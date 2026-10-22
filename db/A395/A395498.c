/*
 * A395498  Number of derangements of [n] with 2 ascents.
 * https://oeis.org/A395498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of derangements of [n] with 2 ascents. Recurrence (order
 *            9): a(n) = 7*a(n-1) -13*a(n-2) -9*a(n-3) +45*a(n-4) -15*a(n-5)
 *            -47*a(n-6) +29*a(n-7) +16*a(n-8) -12*a(n-9) Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=4, a(5)=24, a(6)=120,
 *            a(7)=480, a(8)=1764
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* For small n just return the known value */
    if (n < 9) {
        static const seq_t init[9] = {
            (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
            (seq_t)4, (seq_t)24, (seq_t)120, (seq_t)480, (seq_t)1764
        };
        return init[n];
    }

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* number of computed entries (cache[0 .. cache_len-1]) */

    /* initialise the cache */
    if (!cache) {
        /* Allocate initial capacity */
        size_t init_cap = 16;
        if (init_cap < 9) init_cap = 9;
#ifndef __cplusplus
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#else
        cache = new seq_t[init_cap];
#endif
        cache_cap = init_cap;
        /* copy the first 9 known terms */
        static const seq_t init[9] = {
            (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
            (seq_t)4, (seq_t)24, (seq_t)120, (seq_t)480, (seq_t)1764
        };
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[i];
        cache_len = 9;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
#ifndef __cplusplus
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
#else
            seq_t *tmp = new seq_t[new_cap];
            if (cache) {
                for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
                delete [] cache;
            }
            cache = tmp;
#endif
            cache_cap = new_cap;
        }

        /* Compute terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] =
                (7*cache[i-1]) -
                (13*cache[i-2]) -
                (9*cache[i-3]) +
                (45*cache[i-4]) -
                (15*cache[i-5]) -
                (47*cache[i-6]) +
                (29*cache[i-7]) +
                (16*cache[i-8]) -
                (12*cache[i-9]);
        }
        cache_len = n + 1;
    }

    return cache[n];
}
