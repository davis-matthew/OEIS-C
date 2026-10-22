/*
 * A375255  Expansion of 1/(1 - 2*x + 3*x^2 + 2*x^3 + x^4).
 * https://oeis.org/A375255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) - 2*a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            2, a(2) = 1, a(3) = -6
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure at least four initial values are stored */
    if (cache_len < 4) {
        idx_t new_len = 4;
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        }
        /* initialise the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-6;
        cache_len = new_len;
    }

    /* extend the cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     - 3 * cache[i - 2]
                     - 2 * cache[i - 3]
                     -   cache[i - 4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
