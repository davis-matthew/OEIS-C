/*
 * A370176  a(n) = floor(x*a(n-1)) for n > 0 where x = 3+sqrt(15), a(0) = 1.
 * https://oeis.org/A370176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 6*a(n-3) (n >= 3); a(0)=1, a(1)=6, a(2)=41
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cachesize = 0;

    /* the value is already stored?  */
    if (n < cachesize)
        return cache[n];

    /* allocate space for indices 0 … n  */
    size_t newsize = (size_t)n + 1;
    seq_t *newcache = (seq_t*)malloc(newsize * sizeof(seq_t));
    if (!newcache)
        return 0;                      /* out‑of‑memory: give up */

    /* copy previously computed values */
    if (cache) {
        for (size_t i = 0; i < cachesize; ++i)
            newcache[i] = cache[i];
        free(cache);
    }

    /* commit the new buffer */
    cache     = newcache;
    cachesize = newsize;

    /* initialise the first elements when they are in range */
    if (cachesize > 0) cache[0] = 1;
    if (cachesize > 1) cache[1] = 6;
    if (cachesize > 2) cache[2] = 41;

    /* compute terms up to n using the recurrence */
    for (idx_t i = 3; i <= n; ++i)
        cache[i] = (seq_t)(7 * cache[i - 1] - 6 * cache[i - 3]);

    return cache[n];
}
