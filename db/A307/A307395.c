/*
 * A307395  Expansion of 1/((1 - x) * ((1 - x)^3 + x^3)).
 * https://oeis.org/A307395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 3*a(n-3) for n > 2 Initial values:
 *            a(0)=1, a(1)=4, a(2)=10
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 37, i64 n <= 78, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to hold up to index n */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;  /* allocation failure; caller should not rely on this case */

    /* Copy existing entries */
    if (cache) {
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;

    /* Compute missing terms */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 4;
        else if (i == 2)
            cache[i] = 10;
        else
            cache[i] = (seq_t)4 * cache[i-1]
                     - (seq_t)6 * cache[i-2]
                     + (seq_t)3 * cache[i-3];
    }

    return cache[n];
}
