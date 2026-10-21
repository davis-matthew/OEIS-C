/*
 * A290911  p-INVERT of the positive integers, where p(S) = 1 - 6*S^2.
 * https://oeis.org/A290911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A290911 – p-INVERT of the positive integers with p(S)=1-6S^2
 *
 * Recurrence (order 4):
 *   a(0) = 0
 *   a(1) = 6
 *   a(2) = 24
 *   a(3) = 96
 *   a(n) = 4*a(n-1) + 4*a(n-3) - a(n-4)   for n ≥ 4
 *
 * The function caches computed values so that calls in any order are O(n)
 * in total and each term is computed once.
 */

#include <stdlib.h>  /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache – grows only when needed */
    static seq_t *cache = NULL;     /* dynamically allocated cache array */
    static idx_t  cache_count = 0;  /* number of cached entries (size of cache) */

    /* Ensure cache has enough space to hold a[n] */
    if (n >= cache_count) {
        size_t new_cap = (size_t)(n + 1);

        /* Allocate or enlarge the cache */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory: return 0 (unlikely for n≤10000) */

        /* Compute missing values starting from the first missing index */
        for (idx_t i = cache_count; i <= n; ++i) {
            if (i == 0)       cache[0] = (seq_t)0;
            else if (i == 1)  cache[1] = (seq_t)6;
            else if (i == 2)  cache[2] = (seq_t)24;
            else if (i == 3)  cache[3] = (seq_t)96;
            else
                cache[i] = 4*cache[i-1] + 4*cache[i-3] - cache[i-4];
        }

        cache_count = new_cap;
    }

    return cache[n];
}
