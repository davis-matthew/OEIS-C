/*
 * A313442  Coordination sequence Gal.6.453.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A313442: Coordination sequence Gal.6.453.3
   Recurrence: a(n) = 2*a(n-5) - a(n-10) for n > 13
   Initial values (n = 0..13):
     1, 5, 10, 13, 20, 26, 31, 35, 38, 46, 52, 57, 59, 62 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t base_vals[14] = {
    1, 5, 10, 13, 20, 26, 31, 35, 38, 46, 52, 57, 59, 62
};

static seq_t *cache = NULL;          /* dynamic array containing computed values */
static size_t cache_capacity = 0;    /* allocated size of cache */
static size_t cache_len = 0;          /* number of valid elements currently stored */

/* Ensure cache has at least 'needed' capacity, growing if necessary. */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                   /* fatal if allocation fails */
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n < 14) return base_vals[(size_t)n];

    /* First time we're called with n >= 14: initialise cache */
    if (cache == NULL) {
        ensure_capacity( (size_t)n + 1 );
        for (size_t i = 0; i < 14; ++i) cache[i] = base_vals[i];
        cache_len = 14;
    } else if ((size_t)n + 1 > cache_capacity) {
        ensure_capacity( (size_t)n + 1 );
    }

    /* Compute any missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)2 * cache[i-5] - cache[i-10];
    }
    if ((size_t)n + 1 > cache_len) cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
