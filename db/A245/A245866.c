/*
 * A245866  Number of length n+2 0..5 arrays with some pair in every
 *          consecutive three terms totalling exactly 5.
 * https://oeis.org/A245866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - a(n-3) (n > 3) * Initial values:
 *            a(1) = 90, a(2) = 318, a(3) = 960 * The sequence is indexed
 *            starting at 1. * This implementation memoises computed terms in
 *            a static array to allow * arbitrary order calls
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, realloc, free */

seq_t a(idx_t n)
{
    /* Return 0 for an undefined n = 0.  (Offset begins at 1.) */
    if (n == 0)
        return (seq_t)0;

    /* Static storage for the memoised values. */
    static seq_t *cache = NULL;   /* array indices 0..last_computed */
    static idx_t last_computed = 0; /* highest index with a known value */
    static idx_t cache_size = 0;    /* allocated size */

    /* Ensure the cache has room for at least one entry. */
    if (cache == NULL) {
        cache_size = (idx_t)4;          /* at least indices 0..3 */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL)              /* allocation failed */
            return (seq_t)0;

        /* Initialise known initial terms. */
        cache[1] = (seq_t)90;
        cache[2] = (seq_t)318;
        cache[3] = (seq_t)960;
        last_computed = 3;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)  /* reallocation failed */
            return (seq_t)0;
        /* Zero new elements (for safety, though they'll be overwritten). */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 1]
                     + (seq_t)4 * cache[i - 2]
                     - cache[i - 3];
        }
        last_computed = n;
    }

    /* Return the requested value. */
    return cache[n];
}
