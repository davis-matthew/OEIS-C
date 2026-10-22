/*
 * A309021  Expansion of x * Product_{k>=0} (1 + x^(2^k) - x^(2^(k+1)) -
 *          x^(2^(k+2))).
 * https://oeis.org/A309021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=1; a(2*n) = a(n) a(2*n+1) = a(n+1) - a(n) - a(n-1)
 *            (n≥1)
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to store all values that have been computed so far */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL)  /* allocation failed – abort safely */
            return (seq_t)0;
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* if the requested value is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* grow the cache so that it covers indices up to 'n' */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)  /* realloc failed – abort safely */
        return (seq_t)0;
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_size; i < new_size; ++i) {
        if ((i & 1U) == 0U) {             /* even index: a(2*k) = a(k) */
            cache[i] = cache[i >> 1];
        } else {                           /* odd index: a(2*k+1) = a(k+1) - a(k) - a(k-1) */
            size_t k = (i - 1U) >> 1U;     /* k = (i-1)/2  */
            /* k >= 1 here because i >= 3; thus k-1 is valid */
            cache[i] = cache[k + 1U] - cache[k] - cache[k - 1U];
        }
    }
    cache_size = new_size;
    return cache[n];
}
