/*
 * A375880  a(n) = 2^(2n+1)*cos(nA), where A is the angle opposite side BC in
 *          a triangle ABC having sidelengths |BC|=2, |CA| = 3, |AB| = 4; ABC
 *          is the smallest integer-sided scalene triangle.
 * https://oeis.org/A375880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Sequence A375880:
   a(0) = 2, a(1) = 7,
   a(n) = 7*a(n-1) - 16*a(n-2)   for n >= 2 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of elements currently stored  */

    /* grow cache if needed */
    if (cache_size <= n) {
        /* allocate space for indices 0 .. n */
        seq_t *new_cache = (seq_t *)realloc(cache,
                       (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)        /* allocation failed – return 0 */
            return (seq_t)0;
        cache = new_cache;

        /* initialise first two terms on first allocation */
        if (cache_size == 0) {
            cache[0] = (seq_t)2;
            if (n >= 1) cache[1] = (seq_t)7;
            cache_size = (n >= 1) ? 2 : 1;
        }

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)2;
            } else if (i == 1) {
                cache[1] = (seq_t)7;
            } else {
                cache[i] = (seq_t)7 * cache[i-1] - (seq_t)16 * cache[i-2];
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
