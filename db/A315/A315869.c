/*
 * A315869  Coordination sequence Gal.5.247.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-7) + a(n-8) - a(n-15) for n > 17, initial values:
 *            a(0)=1, a(1)=6, a(2)=8, a(3)=12, a(4)=20, a(5)=24, a(6)=26,
 *            a(7)=32, a(8)=40, a(9)=38, a(10)=50, a(11)=48, a(12)=60,
 *            a(13)=56, a(14)=66, a(15)=72, a(16)=72, a(17)=78
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init[18] = {
    1, 6, 8, 12, 20, 24, 26, 32,
    40, 38, 50, 48, 60, 56, 66, 72,
    72, 78
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* allocated size of cache */
static size_t cache_len = 0;  /* number of entries that have been computed */

/* Allocate cache and copy the initial values. */
static void initialize_cache(void) {
    if (cache) return;
    cache_cap = 18;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    for (size_t i = 0; i < cache_cap; ++i) cache[i] = init[i];
    cache_len = cache_cap;
}

/* Grow the cache so that it can hold index n. */
static void grow_cache(idx_t n) {
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    initialize_cache();   /* ensure cache is ready */

    if ((size_t)n < 18) return init[(size_t)n];

    grow_cache(n);  /* make sure we can index up to n */

    /* Compute missing terms up to n. */
    if (cache_len <= (size_t)n) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            cache[(size_t)i] = cache[(size_t)(i - 7)] +
                               cache[(size_t)(i - 8)] -
                               cache[(size_t)(i - 15)];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
