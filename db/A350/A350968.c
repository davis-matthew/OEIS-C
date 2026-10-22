/*
 * A350968  a(n) = (A350967(n)-1)/2.
 * https://oeis.org/A350968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n)-1)/2, where b(1)=9, b(2)=999 and b(n) =
 *            110*b(n-1)-b(n-2). Equivalently a(1)=4, a(2)=499 and a(n) =
 *            110*a(n-1) - a(n-2) + 54 (n ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard library for malloc/realloc. */
#include <stdlib.h>

/* Static cache to avoid recomputing terms. */
static seq_t *cache = NULL;      /* Array indexed from 1. */
static idx_t  cache_cap = 0;     /* Current capacity (size of array). */
static idx_t  last      = 0;     /* Highest n that has been computed. */

seq_t a(idx_t n)
{
    /* For indices below the offset behavior is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the cache is large enough to hold up to n. */
    size_t required = (size_t)n + 1;          /* include index 0 */
    if (required > cache_cap) {
        cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        cache_cap = required;
    }

    /* Extend the cache only if necessary. */
    if (n > last) {
        /* Initialise the first two terms if they have not yet been set. */
        if (last < 1) cache[1] = (seq_t)4;
        if (last < 2 && n >= 2) cache[2] = (seq_t)499;

        /* Compute all missing terms up to n. */
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 2) continue;              /* already initialised */
            cache[i] = 110 * cache[i-1] - cache[i-2] + (seq_t)54;
        }
        last = n;
    }
    return cache[n];
}
