/*
 * A350922  a(0) = 2, a(1) = 5, and a(n) = 7*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 5, a(n) = 7*a(n-1) - a(n-2) - 4 for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* sequence cache */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  max_computed = 0;      /* highest index computed in cache */
static int initialized = 0;          /* flag whether cache initialized */

static void grow_cache(idx_t new_cap) {
    /* Allocate or reallocate cache to at least new_cap elements */
    if (new_cap <= cache_cap)
        return;
    size_t old = cache_cap;
    /* Double until large enough */
    while (new_cap > cache_cap)
        cache_cap = cache_cap == 0 ? 2 : cache_cap * 2;
    if (cache == NULL)
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    /* Initialize new slots (not strictly necessary but safe) */
    for (size_t i = old; i < cache_cap; ++i)
        cache[i] = 0;
}

seq_t a(idx_t n) {
    /* Initialise cache on first call */
    if (!initialized) {
        grow_cache(2);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        max_computed = 1;
        initialized = 1;
    }

    if (n <= max_computed)
        return cache[n];

    grow_cache(n + 1);  /* Ensure capacity up to n */

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1] - cache[i - 2] - (seq_t)4;
    }
    max_computed = n;
    return cache[n];
}
