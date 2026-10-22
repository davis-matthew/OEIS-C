/*
 * A315741  Coordination sequence Gal.6.210.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-6) + 2*a(n-7) - a(n-8); Initial
 *            values: a(0)=1 a(1)=6 a(2)=12 a(3)=18 a(4)=23 a(5)=28 a(6)=33
 *            a(7)=38 a(8)=43
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static int base_done = 0;

    /* Need to grow cache to hold index n */
    idx_t new_size = n + 1;
    idx_t old_size = cache_size;          /* size before possible reallocation */

    if (cache == NULL) {                  /* first call */
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failed */
        cache_size = new_size;
        base_done = 0;
    } else if (new_size > cache_size) {   /* enlarge cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed */
        cache = tmp;
        cache_size = new_size;
        base_done = 0;                    /* old contents may be lost */
    }

    /* Ensure base values are present */
    if (cache_size >= 9) {
        cache[0] = 1;  cache[1] = 6;  cache[2] = 12; cache[3] = 18; cache[4] = 23;
        cache[5] = 28; cache[6] = 33; cache[7] = 38; cache[8] = 43;
        base_done = 1;
    } else {
        if (cache_size > 0) cache[0] = 1;
        if (cache_size > 1) cache[1] = 6;
        if (cache_size > 2) cache[2] = 12;
        if (cache_size > 3) cache[3] = 18;
        if (cache_size > 4) cache[4] = 23;
        if (cache_size > 5) cache[5] = 28;
        if (cache_size > 6) cache[6] = 33;
        if (cache_size > 7) cache[7] = 38;
        if (cache_size > 8) cache[8] = 43;
    }

    /* Compute missing terms up to n */
    if (n > 8) {
        /* If we grew, compute from the first index not yet computed */
        idx_t start = (old_size > 9) ? old_size : 9;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = cache[i-1] * 2;   /* 2*a(i-1) */
            val -= cache[i-2];            /* - a(i-2) */
            val -= cache[i-6];            /* - a(i-6) */
            val += cache[i-7] * 2;        /* + 2*a(i-7) */
            val -= cache[i-8];            /* - a(i-8) */
            cache[i] = val;
        }
    }

    return cache[n];
}
