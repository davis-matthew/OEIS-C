/*
 * A295861  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = -1, a(2) = 0, a(3) = 1.
 * https://oeis.org/A295861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4); Initial values:
 *            a(0) = -2, a(1) = -1, a(2) = 0, a(3) = 1
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Memoisation cache – static across calls */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_len = 0;    /* current cached size */

    /* Initialise cache with the first four terms on first call */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory → zero */
        cache[0] = (seq_t)-2;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t) 0;
        cache[3] = (seq_t) 1;
    }

    /* Ensure the cache can hold the requested index */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory → zero */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1]
                 + 3 * cache[i-2]
                 - 2 * cache[i-3]
                 - 2 * cache[i-4];
    }

    return cache[(size_t)n];
}
