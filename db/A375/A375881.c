/*
 * A375881  a(n) = 2^(3n+1)*cos(nB), where B is the angle opposite side CA in
 *          a triangle ABC having sidelengths |BC|=2, |CA| = 3, |AB| = 4; ABC
 *          is the smallest integer-sided scalene triangle.
 * https://oeis.org/A375881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11*a(n-1)-64*a(n-2), a(0) = 2, a(1) = 11
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of entries currently allocated */

    /* base cases handled directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)11;

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory; caller can check for 0 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)11;
    }

    /* extend cache to at least n+1 entries */
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size * 2 : 4;
        if (new_size <= n + 1) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* out‑of‑memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms upwards */
    for (idx_t i = (cache_size > 2 ? 2 : 0); i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)11 * prev1 - (seq_t)64 * prev2;
    }

    return cache[n];
}
