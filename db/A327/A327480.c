/*
 * A327480  a(n) is the maximum number of squares of unit area that can be
 *          removed from an n X n square while still obtaining a connected
 *          figure without holes and of the longest perimeter.
 * https://oeis.org/A327480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) for n >
 *            10, with initial values: a(0)=0, a(1)=0, a(2)=2, a(3)=4, a(4)=8,
 *            a(5)=12, a(6)=22, a(7)=28, a(8)=40, a(9)=48, a(10)=64
 * Range:     u8 n <= 19, u16 n <= 296, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of elements already computed */
    static size_t cache_cap = 0;   /* allocated capacity (number of slots) */

    /* ensure cache has capacity for indices up to n */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first 11 terms if not already done */
    if (cache_len <= 10) {
        if (cache_cap < 11) {
            cache = (seq_t *)realloc(cache, 11 * sizeof(seq_t));
            cache_cap = 11;
        }
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 4;
        cache[4] = 8;
        cache[5] = 12;
        cache[6] = 22;
        cache[7] = 28;
        cache[8] = 40;
        cache[9] = 48;
        cache[10] = 64;
        cache_len = 11;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-4) - 2*a(i-5) + a(i-6) */
        seq_t t = 2 * cache[i - 1]
                - cache[i - 2]
                + cache[i - 4]
                - 2 * cache[i - 5]
                + cache[i - 6];
        cache[i] = t;
    }
    cache_len = n + 1;
    return cache[n];
}
