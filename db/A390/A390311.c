/*
 * A390311  a(n) = n!*(4^(n+1) - 3^(n+1)).
 * https://oeis.org/A390311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (4^(n+1) - 3^(n+1)) Also satisfies the recurrence:
 *            a(0) = 1 a(1) = 7 a(n) = 7*n*a(n-1) - 12*(n-1)*n*a(n-2) for n >=
 *            2 The recurrence is derived from the closed form and is O(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage: the function must be visible to the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memoization.  The cache is grown on demand and
       reused across calls, so the function works when calls are not in
       increasing order. */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* allocated size */
    static idx_t  len = 0;         /* number of computed terms */

    /* Helper to enlarge the cache. */
    if (cache == NULL) {
        /* Allocate at least two positions for the base cases. */
        cap = (n >= 1) ? 2 : 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) {
            /* In case of allocation failures, return zero. */
            return (seq_t)0;
        }
        /* Initialise base cases. */
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)7;
        len = (cap > 1) ? 2 : 1;
    } else if ((size_t)n >= cap) {
        /* Grow the cache until it can hold the requested index. */
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute up to n if we haven't yet. */
    for (idx_t k = len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)7;
        } else {
            /* recurrence: a(k) = 7*k*a(k-1) - 12*(k-1)*k*a(k-2) */
            seq_t term1 = (seq_t)7 * (seq_t)k * cache[k-1];
            seq_t term2 = (seq_t)12 * (seq_t)(k-1) * (seq_t)k * cache[k-2];
            cache[k]   = term1 - term2;
        }
    }

    len = (n + 1 > len) ? n + 1 : len;
    return cache[n];
}
