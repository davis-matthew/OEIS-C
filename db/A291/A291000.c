/*
 * A291000  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3.
 * https://oeis.org/A291000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) (n ≥ 3) with initial
 *            values a(0)=1, a(1)=3, a(2)=9, a(3)=26 This is sequence A291000
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache    = NULL; /* holds computed terms */
    static size_t cache_cap = 0;   /* allocated size of cache */
    static idx_t  max_computed = (idx_t)-1; /* highest index computed */
    static bool   initialized = false;

    /* initialise cache on first call */
    if (!initialized) {
        cache_cap = 4;               /* we need indices 0..3 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory, but not expected for this test */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)26;
        max_computed = 3;
        initialized = true;
    }

    /* if the requested value is already in the cache, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* grow cache if necessary (doubling strategy for amortised O(n)) */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while ((size_t)(n + 1) > new_cap) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory, not expected */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 4 * cache[i-1];
        val -= 4 * cache[i-2];
        val += 2 * cache[i-3];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
