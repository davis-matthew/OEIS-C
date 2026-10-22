/*
 * A342403  a(1) = 1; a(n) = -Sum_{d|n, d < n} d * a(d).
 * https://oeis.org/A342403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n,d<n} d * a(d) Initial value: a(1) = 1
 * Range:     i8 n <= 23, i16 n <= 71, i32 n <= 383, i64 n <= 3839,
 *            i128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] unused, cache[1..] holds computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated array size (in seq_t elements) */
    static idx_t cache_high = 0;    /* largest n that has been computed */

    if (n < 1)        /* offsets below 1 are irrelevant */
        return 0;

    /* allocate or grow the cache so that index n is valid */
    if (cache == NULL) {
        cache_size = 2;                      /* indices 0..1 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;
        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)1;                /* a(1) = 1 */
        cache_high = 1;
    }
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* If the required value is already cached, return it */
    if (n <= cache_high)
        return cache[n];

    /* Compute all missing values up to n */
    for (idx_t k = cache_high + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t d = 1; d <= k / d; ++d) {    /* iterate over divisors ≤ √k */
            if (k % d == 0) {
                if (d < k)
                    sum += (seq_t)d * cache[d];
                idx_t e = k / d;
                if (e != d && e < k)
                    sum += (seq_t)e * cache[e];
            }
        }
        cache[k] = -sum;
    }
    cache_high = n;
    return cache[n];
}
