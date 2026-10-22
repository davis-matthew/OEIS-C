/*
 * A315672  Coordination sequence Gal.5.139.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10), with
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=21, a(5)=26, a(6)=31,
 *            a(7)=35, a(8)=40, a(9)=46, a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Convert n to a suitable index type (size_t) */
    size_t idx = (size_t)n;

    /* Lazy initialization of cache with the first 11 terms */
    if (cache == NULL) {
        cache_cap = 64; /* initial capacity, will grow if needed */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0; /* allocation failed, return 0 as safe fallback */

        cache[0]  = 1;
        cache[1]  = 6;
        cache[2]  = 12;
        cache[3]  = 17;
        cache[4]  = 21;
        cache[5]  = 26;
        cache[6]  = 31;
        cache[7]  = 35;
        cache[8]  = 40;
        cache[9]  = 46;
        cache[10] = 52;

        cache_len = 11;
    }

    /* If the requested index is already cached, return it */
    if (idx < cache_len)
        return cache[idx];

    /* Ensure we have enough capacity, grow if necessary */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= idx + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to idx using the recurrence relation */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val =  2 * cache[i-1] - 2 * cache[i-2] + 2 * cache[i-3]
                    - 2 * cache[i-4] + 2 * cache[i-5] - 2 * cache[i-6]
                    + 2 * cache[i-7] - 2 * cache[i-8] + 2 * cache[i-9]
                    - cache[i-10];
        cache[i] = val;
    }
    cache_len = idx + 1;
    return cache[idx];
}
