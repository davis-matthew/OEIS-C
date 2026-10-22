/*
 * A315538  Coordination sequence Gal.3.50.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 4; a(0)=1, a(1)=6,
 *            a(2)=11, a(3)=17, a(4)=23
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;      /* allocated size in elements */
static idx_t max_computed = 0;    /* highest index that has been filled */

/* Ensure the cache contains values up to at least n, computing missing
   terms on demand.  The function is safe for any order of calls. */
static void
ensure_cache(idx_t n)
{
    if (n < 5) {
        /* No caching needed for the first five terms. */
        return;
    }

    /* Allocate and initialise the cache if it hasn't been already. */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        cache[0] = 1;  /* a(0) */
        cache[1] = 6;  /* a(1) */
        cache[2] = 11; /* a(2) */
        cache[3] = 17; /* a(3) */
        cache[4] = 23; /* a(4) */
        max_computed = 4;
    }

    /* Grow the cache if necessary. */
    while (cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)(n + 1))
            new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* In a real-world scenario we would handle the allocation
               failure; for this problem we assume it never fails. */
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms that are still missing. */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 3];
            cache[i] = sum - cache[i - 4];
        }
        max_computed = n;
    }
}

/* Public interface.  Computes a(n) for any nonnegative n, with O(n)
   complexity and memoisation to avoid recomputation. */
seq_t
a(idx_t n)
{
    if (n < 5) {
        static const seq_t init[5] = {1, 6, 11, 17, 23};
        return init[n];
    }

    ensure_cache(n);
    return cache[n];
}
