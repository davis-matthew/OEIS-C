/*
 * A307374  G.f. A(x) satisfies: A(x) = 1 + x - x^2*A(x)^2.
 * https://oeis.org/A307374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 34, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1, a(1) = 1;
   a(n+2) = -∑_{k=0..n} a(k) * a(n-k)  (for n ≥ 0) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache of computed a(i) */
    static idx_t cache_len = 0;   /* number of elements in cache */

    /* initialise cache with first two terms if necessary */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }

    /* extend cache up to index n */
    if ((idx_t)(n + 1) > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort the program */
            /* For safety in a library context, we simply return 0. */
            return 0;
        }
        cache = tmp;
        /* compute the missing terms */
        for (idx_t m = cache_len; m <= n; ++m) {
            seq_t sum = 0;
            /* m >= 2 because cache_len at least 2 and m grows from cache_len upward */
            for (idx_t k = 0; k <= m - 2; ++k) {
                sum += cache[k] * cache[(m - 2) - k];
            }
            cache[m] = -sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
