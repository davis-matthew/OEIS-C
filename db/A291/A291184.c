/*
 * A291184  p-INVERT of the positive integers, where p(S) = 1 - 4*S + 3*S^2.
 * https://oeis.org/A291184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4, a(1) = 21, a(2) = 104, a(3) = 507 a(n) = 8*a(n-1) -
 *            17*a(n-2) + 8*a(n-3) - a(n-4) for n >= 4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global memoization storage */
static seq_t *cache = NULL;          /* array holding computed terms */
static size_t cache_size = 0;        /* allocated capacity of cache    */
static size_t computed = 3;          /* index of the highest computed term */

/* Initial values for n = 0..3 */
static const seq_t init_vals[4] = {4, 21, 104, 507};

seq_t a(idx_t n)
{
    /* Return directly for the first four terms */
    if (n < 4) {
        return init_vals[n];
    }

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* Ensure capacity at least n+1, but start reasonably small */
        cache_size = (size_t)n + 1;
        if (cache_size < 4) cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) { /* allocation failure: return 0 as a fallback */
            return 0;
        }
        /* Copy the first four terms */
        for (idx_t i = 0; i < 4; ++i) {
            cache[i] = init_vals[i];
        }
        /* Compute up to n */
        for (idx_t i = 4; i <= n; ++i) {
            cache[i] = 8 * cache[i-1]
                       - 17 * cache[i-2]
                       + 8 * cache[i-3]
                       - cache[i-4];
        }
        computed = (size_t)n;
        return cache[n];
    }

    /* Extend cache if the requested term exceeds current capacity */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = cache_size * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 as a fallback */
            return 0;
        }
        cache = tmp;
        cache_size = new_cap;
    }

    /* If the term has already been computed, just return it */
    if ((size_t)n <= computed) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
        cache[i] = 8 * cache[i-1]
                   - 17 * cache[i-2]
                   + 8 * cache[i-3]
                   - cache[i-4];
    }
    computed = (size_t)n;
    return cache[n];
}
