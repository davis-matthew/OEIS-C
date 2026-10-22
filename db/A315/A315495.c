/*
 * A315495  Coordination sequence Gal.5.135.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-9) - a(n-10)  for n > 10
 * Initial values (offset 0):
 * a(0) = 1
 * a(1) = 6
 * a(2) = 11
 * a(3) = 16
 * a(4) = 22
 * a(5) = 28
 * a(6) = 34
 * a(7) = 39
 * a(8) = 44
 * a(9) = 50
 * a(10) = 56
 */
#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[11] = {
    (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)16, (seq_t)22,
    (seq_t)28, (seq_t)34, (seq_t)39, (seq_t)44, (seq_t)50,
    (seq_t)56
};

seq_t a(idx_t n)
{
    /* static cache to memoize computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of values actually stored */
    static size_t cache_cap  = 0;   /* capacity of allocated array */

    /* allocate and initialise cache on first call */
    if (cache == NULL) {
        /* start with enough space for the initial 11 values */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* allocation failure */
        for (size_t i = 0; i < 11; ++i) cache[i] = init_vals[i];
        cache_len = 11;
    }

    /* If requested term already computed, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache if it is not large enough */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 11) {
            cache[i] = init_vals[i];
        } else {
            /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10) */
            cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
        }
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
