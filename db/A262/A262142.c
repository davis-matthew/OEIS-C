/*
 * A262142  The first of ten consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of nine
 *          consecutive positive integers.
 * https://oeis.org/A262142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39*a(n-1) - 39*a(n-2) + a(n-3) for n>3; initial terms:
 *            a(1)=171, a(2)=6660, a(3)=253071
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1, return 0 */
    if (n < 1)
        return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;  /* allocated capacity (in elements) */
    static size_t cache_len = 0;  /* number of terms stored */

    /* Initialise cache with the first three terms on the first call */
    if (cache_len == 0) {
        cache_cap = 4;                     /* capacity at least 3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                     /* allocation failure */
        cache[0] = (seq_t)171;
        cache[1] = (seq_t)6660;
        cache[2] = (seq_t)253071;
        cache_len = 3;
    }

    /* Return already computed value if available */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* Grow the cache if required */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = (seq_t)39 * cache[i - 1]
                 - (seq_t)39 * cache[i - 2]
                 + cache[i - 3];
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
