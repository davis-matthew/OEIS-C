/*
 * A367400  Number of subsets of {1..n} whose cardinality is not the sum of
 *          two distinct elements.
 * https://oeis.org/A367400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) for n > 3, with
 *            a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;     /* pointer to cached values */
static idx_t   cache_size = 0;  /* number of valid entries in cache */

/* Helper to resize the cache array */
static void grow_cache(idx_t new_size) {
    if (cache == NULL) {
        /* Allocate a new array */
        cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    } else {
        /* Reallocate to the larger size */
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    }
    /* In case realloc failed, assume memory is still valid for this use-case */
    cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure we have space for at least n+1 entries */
    if (cache_size <= n) {
        /* Request at least 4 entries to initialize base cases */
        idx_t required = n + 1;
        if (required < 4) required = 4;
        grow_cache(required);
        /* Initialize base cases once */
        if (cache_size == required) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)7;
        }
    }

    /* If n is within initialized range, return immediately */
    if (n < cache_size && n < 4) {
        return cache[n];
    }
    /* Compute missing values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = (cache[i-1] << 1) + (cache[i-3] << 1);
        term -= cache[i-2];
        term -= cache[i-4];
        cache[i] = term;
    }
    return cache[n];
}
