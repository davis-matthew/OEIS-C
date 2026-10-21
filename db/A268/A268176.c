/*
 * A268176  a(0) = a(1) = 1, and a(n) = a(n-1) + a( (a(n-1)-1) mod n ) for
 *          n>=2.
 * https://oeis.org/A268176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1; for n>=2, a(n) = a(n-1) + a( (a(n-1)-1) mod
 *            n )
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 138, u64 n <= 627,
 *            u128 n <= 2412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* pointers to stored terms            */
static idx_t cache_size = 0;          /* first unset index (size of cache)   */
static idx_t cache_capacity = 0;      /* allocated array size (>=cache_size) */

/* Ensure the cache can hold indices up to max_idx (inclusive). */
static void ensure_cache(idx_t max_idx) {
    if (max_idx < cache_capacity) return;

    /* Grow capacity exponentially */
    idx_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap <= max_idx) new_cap <<= 1;

    /* Reallocate memory */
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) abort();            /* out of memory */

    cache = new_mem;
    cache_capacity = new_cap;
}

/* Return sequence value a(n). */
seq_t a(idx_t n) {
    /* Initialise cache on first call */
    if (!cache) {
        ensure_cache(1);             /* at least capacity 2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    if (n < cache_size) return cache[n];

    /* Compute new values up to n */
    ensure_cache(n);
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t m     = prev - (seq_t)1;
        idx_t pos   = (idx_t)(m % (seq_t)i);   /* (a(n-1)-1) mod n */
        cache[i]    = prev + cache[pos];
    }
    cache_size = n + 1;
    return cache[n];
}
