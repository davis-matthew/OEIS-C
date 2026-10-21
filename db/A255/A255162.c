/*
 * A255162  Rational part of circle radii in nested circles and hexagons (see
 *          comment).
 * https://oeis.org/A255162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 17, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2, a(1) = 0,
   a(n) = -12 * a(n-1) + 12 * a(n-2)  (n >= 2) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache holds values that have already been computed */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t idx = (size_t)n;              /* index as a size_t for array access */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* out‑of‑memory guard */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* extend the cache up to the requested index, if needed */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* out‑of‑memory guard */
        cache = tmp;
        for (size_t i = cache_size; i <= idx; ++i) {
            cache[i] = (seq_t)(-12) * cache[i - 1] + (seq_t)12 * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[idx];
}
