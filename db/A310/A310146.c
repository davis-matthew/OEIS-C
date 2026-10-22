/*
 * A310146  Coordination sequence Gal.5.39.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-13) - a(n-16) (n > 16) initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=11, a(5)=14, a(6)=17,
 *            a(7)=20, a(8)=24, a(9)=26, a(10)=28, a(11)=32, a(12)=34,
 *            a(13)=37, a(14)=41, a(15)=43, a(16)=46
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL; /* pointer to stored values */
static idx_t   cache_size = 0; /* number of elements currently computed */
static idx_t   cache_cap  = 0; /* capacity of the allocated array */

/* Allocate or grow the cache to hold at least 'n+1' elements. */
static void
ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;
    /* grow by factor 2 until sufficient */
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Precomputed base values a(0) … a(16). */
static const seq_t base_vals[17] = {
    1, 3, 6,  9, 11, 14, 17, 20, 24, 26,
   28, 32, 34, 37, 41, 43, 46
};

/* Compute and return a(n).  Can be called in any order. */
seq_t a(idx_t n)
{
    /* Initialize cache on first call. */
    if (cache_size == 0) {
        ensure_capacity(16);
        for (size_t i = 0; i < 17; ++i)
            cache[i] = base_vals[i];
        cache_size = 17;
    }

    /* Expand cache up to requested index. */
    if (n >= cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-3) + a(i-13) - a(i-16) */
            cache[i] = cache[i-3] + cache[i-13] - cache[i-16];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
