/*
 * A375885  a(n) = 4^n sin(nC)/sqrt(15), where B is the angle opposite side CA
 *          in a triangle ABC having sidelengths |BC|=2, |CA| = 3, |AB| = 4;
 *          ABC is the smallest integer-sided scalene triangle.
 * https://oeis.org/A375885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 2*a(n‑1) - 16*a(n‑2) for n >= 2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* first call: initialise the cache with the two base cases */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* if the value is already cached, simply return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* extend the cache so that it can hold index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory */
        /* compute the new terms using the recurrence */
        for (size_t i = cache_len; i < needed; ++i) {
            cache[i] = (seq_t)(2 * cache[i - 1] - 16 * cache[i - 2]);
        }
        cache_len = needed;
    }

    return cache[n];
}
