/*
 * A362278  Expansion of e.g.f. exp(x - 3*x^2/2).
 * https://oeis.org/A362278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 15, i64 n <= 25, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362278:
   a(0) = 1, a(1) = 1
   a(n) = a(n-1) - 3*(n-1)*a(n-2)   for n > 1
   (exp. generating function: exp(x - 3*x^2/2)) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i).  Cache is grown lazily. */
    static seq_t *cache  = NULL;
    static size_t cache_len = 0;      /* number of cached elements */

    /* Initialise cache with first two terms if not yet done. */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(NULL, 2 * sizeof(seq_t));
        if (cache == NULL)            /* allocation failed */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* If the requested term is already cached, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Extend the cache up to index n (inclusive). */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)                     /* allocation failed */
        return (seq_t)0;
    cache = tmp;

    /* Compute missing terms using the recurrence.  The loop starts at
       the first uncomputed index, i.e. cache_len. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* a(i) = a(i-1) - 3*(i-1)*a(i-2) */
        seq_t prev     = cache[i - 1];
        seq_t prevprev = cache[i - 2];

        seq_t term = prevprev;
        term *= (seq_t)(i - 1);   /* multiply by (i-1) */
        term *= (seq_t)3;         /* multiply by 3 */

        cache[i] = prev - term;
    }

    cache_len = new_len;
    return cache[n];
}
