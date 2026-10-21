/*
 * A280761  Solutions y_n to the negative Pell equation y^2 = 72*x^2 - 8.
 * https://oeis.org/A280761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) with a(0) = 8, a(1) = 280
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t types are provided by the build environment */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;        /* array of computed values */
    static size_t cache_cap = 0;       /* allocated size of cache */
    static idx_t max_computed = 0;     /* largest index for which cache[i] is valid */

    /* Special case: n == 0 */
    if (n == 0) return (seq_t)8;

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate space for at least indices 0 and 1.  Resize later if needed. */
        cache_cap = 2;
        if (cache_cap <= n) cache_cap = n + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();          /* fatal */
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)280;
        max_computed = 1;
    }

    /* Grow cache if we need to compute beyond the current maximum */
    if (n > max_computed) {
        /* Grow memory block */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();           /* fatal */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute new terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t temp = 34 * cache[i - 1];
            temp -= cache[i - 2];
            cache[i] = temp;
        }
        max_computed = n;
    }

    /* Return the cached value */
    return cache[n];
}
