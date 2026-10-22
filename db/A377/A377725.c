/*
 * A377725  Length of the short leg of the unique primitive Pythagorean triple
 *          whose inradius is A002315(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A377725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3), a(0)=3, a(1)=15, a(2)=83
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* pointer to cached values        */
    static idx_t  cache_size = 0; /* number of cached entries         */

    /* initialise cache on first call with the first three values */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure   */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)83;
    }

    /* if the requested value is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache up to n (inclusive) */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) return (seq_t)0;          /* allocation failure   */

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i-1]
                 - (seq_t)7 * cache[i-2]
                 + cache[i-3];
    }
    cache_size = n + 1;

    return cache[n];
}
