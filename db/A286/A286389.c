/*
 * A286389  a(0) = 0; a(n) = n - a(floor(a(n-1)/2)).
 * https://oeis.org/A286389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n - a(floor(a(n-1)/2))
 * Range:     u8 n <= 348, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[i] holds a(i); cache_size is the number of elements stored. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of computed entries */

    /* initialise the cache with the first term */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache_size = 1;
    }

    /* extend the cache (and compute any new values) up to n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            idx_t j = (idx_t)(prev >> 1);           /* floor(prev/2) */
            seq_t val = cache[j];
            cache[i] = (seq_t)i - val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
