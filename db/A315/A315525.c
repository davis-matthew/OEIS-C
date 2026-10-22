/*
 * A315525  Coordination sequence Gal.3.49.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6 Initial values:
 *            a(0)=1, a(1)=6, a(2)=11, a(3)=17, a(4)=22, a(5)=28, a(6)=34
 *            Offset: 0
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoisation array */
static size_t cache_cap = 0;      /* current capacity in elements */
static idx_t cache_max = (idx_t)-1; /* highest index computed (-1 means none yet) */

static void ensure_computed(idx_t n) {
    /* Grow the cache array to hold indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        if (new_cap < needed) new_cap = needed;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Initialise first 7 values once */
    if (cache_max == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)28;
        cache[6] = (seq_t)34;
        cache_max = 6;
    }
    /* Compute any missing values up to n */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[(size_t)i] = cache[(size_t)(i-1)]
                               + cache[(size_t)(i-5)]
                               - cache[(size_t)(i-6)];
        }
        cache_max = n;
    }
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return cache[(size_t)n];
}
