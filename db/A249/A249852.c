/*
 * A249852  a(n) is the total number of pentagons on the left or the right of
 *          the vertical symmetry axis of a pentagon expansion (vertex to
 *          vertex) after n iterations.
 * https://oeis.org/A249852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5) with
 *            a(0)=0, a(1)=2, a(2)=7, a(3)=14, a(4)=23. The function caches
 *            computed values for O(1) repeated lookups
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A249852_H
#define A249852_H

#include <stdlib.h>  /* malloc, realloc */

/*  seq_t and idx_t are defined externally.  */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t size  = 0;   /* number of values stored (n+1) */

    /* Initialise base cases on first call */
    if (size == 0) {
        size = 5;          /* a(0) … a(4) */
        cap  = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure handled by caller */
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 7;
        cache[3] = 14;
        cache[4] = 23;
    }

    /* Return cached value if already computed */
    if ((idx_t)n < size) {
        return cache[n];
    }

    /* Grow the cache if needed */
    if ((idx_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute values from current size up to n */
    for (idx_t k = size; k <= n; ++k) {
        seq_t val;
        if (k >= 5) {
            val = 3 * cache[k-1] - 4 * cache[k-2] + 4 * cache[k-3] - 3 * cache[k-4] + cache[k-5];
        } else {
            /* k < 5 should already be present, but keep safe */
            val = cache[k];
        }
        cache[k] = val;
    }

    size = n + 1;   /* update stored count */
    return cache[n];
}

#endif /* A249852_H */
