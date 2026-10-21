/*
 * A262074  The first of seven consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of eight
 *          consecutive positive integers.
 * https://oeis.org/A262074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 31*a(n-2) + a(n-3) for n>3 Initial values:
 *            a(1)=113, a(2)=3473, a(3)=104161
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset and trivial indices first */
    if (n == 0)
        return (seq_t)0;                     /* Undefined, return 0 */

    if (n <= 3) {
        static const seq_t init[4] = {0,
            (seq_t)113,
            (seq_t)3473,
            (seq_t)104161};
        return init[n];
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;              /* largest index currently cached */

    /* Lazily initialise the cache with the first three values */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[1] = (seq_t)113;
        cache[2] = (seq_t)3473;
        cache[3] = (seq_t)104161;
    }

    /* If we already have the requested term, return it */
    if (n <= cache_size)
        return cache[n];

    /* Grow the cache to accommodate the requested index */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0;                      /* allocation failed */
    cache = new_cache;

    /* Compute all missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = (seq_t)31 * cache[i - 1]
                 - (seq_t)31 * cache[i - 2]
                 + cache[i - 3];
    }
    cache_size = n;
    return cache[n];
}
