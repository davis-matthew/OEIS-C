/*
 * A352789  Number of ways to tile a 3 X n strip with squares and P-shaped
 *          heptominoes.
 * https://oeis.org/A352789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 8*a(n-3) + 6*a(n-5) with initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=9, a(4)=17
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Helper to grow cache to at least requested size */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the cache on first use */
    if (cache_size == 0) {
        /* ensure at least 5 entries for the base cases */
        size_t min_size = 5;
        if (min_size > cache_cap) {
            seq_t *new_cache = (seq_t *)malloc(min_size * sizeof(seq_t));
            if (cache) free(cache);
            cache = new_cache;
            cache_cap = min_size;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)17;
        cache_size = 5;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = 0;
        if (i <= 4) {
            val = cache[i];
        } else {
            val = cache[i - 1] + cache[i - 3] * (seq_t)8 + cache[i - 5] * (seq_t)6;
        }
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
