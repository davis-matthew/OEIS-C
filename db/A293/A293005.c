/*
 * A293005  Number of associative, quasitrivial, and order-preserving binary
 *          operations on the n-element set {1,...,n}.
 * https://oeis.org/A293005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2 (a(0)=0, a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          also satisfies  a(n+2)-2*a(n+1)-2*a(n) = 2   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static variables hold the memoised sequence */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated number of slots    */
    static idx_t  last_computed = (idx_t)(-1); /* largest index computed */

    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        /* Ensure we can access at least a(0) and a(1) */
        cache_cap = (size_t)((n >= 1) ? n + 1 : 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();

        cache[0] = (seq_t)0;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        last_computed = (cache_cap > 1) ? 1 : 0;
    }

    /* Grow the cache if the requested index is beyond current capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *resized = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (resized == NULL) abort();
        cache     = resized;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to and including n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Recurrence: a(i) = 2*a(i-1) + 2*a(i-2) + 2 */
            cache[i] = 2 * cache[i - 1] + 2 * cache[i - 2] + 2;
        }
        last_computed = n;
    }

    return cache[n];
}
