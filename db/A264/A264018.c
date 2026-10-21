/*
 * A264018  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,2.
 * https://oeis.org/A264018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-3) + a(n-4) Initial values: a(1)=1,
 *            a(2)=5, a(3)=25, a(4)=105 The implementation memoises previously
 *            computed terms and extends the cache only when necessary. The
 *            cache is never shrunk, so calls in any order remain O(max n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cstddef>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation using a std::vector cache. */
    static std::vector<seq_t> cache;
    static idx_t cache_max = 0;          /* highest index with a value */

    /* Ensure the vector is large enough to hold indices up to 4. */
    if (cache.size() < 5) {
        cache.resize(5);
        /* Initialise the first four terms once. */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)105;
        cache_max = 4;
    }

    if (n > cache_max) {
        /* Extend the vector if needed. */
        if (cache.size() <= (size_t)n) cache.resize((size_t)n + 1);

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1]   /* a(i-1) */
                     + 4 * cache[i - 3]   /* a(i-3) */
                     +    cache[i - 4];   /* a(i-4) */
        }
        cache_max = n;
    }
    return cache[n];
#else   /* C implementation using a dynamically sized static array. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest index with a value */
    static idx_t cache_cap = 0;   /* allocated array capacity */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        /* Allocate at least capacity for index 4. */
        cache_cap = (idx_t)((n > 4) ? n + 1 : 5);
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        /* Guard against allocation failure; return 0 if it occurs. */
        if (cache == NULL) return (seq_t)0;

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)105;
        cache_max = 4;
    }

    /* Grow the array if the requested index exceeds current capacity. */
    if (n >= cache_cap) {
        idx_t new_cap = (n > cache_cap * 2) ? (n + 1) : (cache_cap * 2);
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        /* Initialise the newly added elements to 0. */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1]
                     + 4 * cache[i - 3]
                     +   cache[i - 4];
        }
        cache_max = n;
    }
    return cache[n];
#endif
}
