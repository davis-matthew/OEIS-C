/*
 * A264166  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 0,0 0,1 1,0 -2,-1 or
 *          -1,-2.
 * https://oeis.org/A264166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) + a(n-4) + a(n-5) - a(n-6),
 *            a(1)=1, a(2)=4, a(3)=8, a(4)=16, a(5)=33, a(6)=69. The function
 *            uses a static memoization cache, so each value is computed only
 *            once, regardless of the order of calls
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset 1 – undefined for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache state */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Initial values a(1)..a(6) */
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)4, (seq_t)8,
        (seq_t)16, (seq_t)33, (seq_t)69
    };

    /* First time: allocate cache and store the first 6 terms */
    if (cache_cap == 0) {
        idx_t cap = 8;
        if (cap < 6) cap = 6;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure (unlikely) */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_cap = cap;
        cache_len = 6;
    }

    /* Grow the cache if required (geometric growth) */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            val = init[i - 1];
        } else {
            val = cache[i - 2] + cache[i - 3] +
                  2 * cache[i - 4] + cache[i - 5] + cache[i - 6] -
                  cache[i - 7];
        }
        cache[i - 1] = val;
    }
    if (n > cache_len)
        cache_len = n;

    return cache[n - 1];
}
