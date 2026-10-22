/*
 * A396287  a(n) = a(n-1) + a(n-1)^4 with a(0)=1.
 * https://oeis.org/A396287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-1)^4, a(0) = 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;

    /* Initialise cache with the first term, a(0) = 1 */
    if (cache == NULL) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0]    = (seq_t)1;
        cache_cap   = 1;
    }

    /* If the requested index is beyond the cached values,
       extend the cache and compute the missing terms. */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;

        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            seq_t x  = cache[i - 1];
            seq_t x4 = x * x * x * x;          /* x^4 */
            cache[i] = x + x4;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
