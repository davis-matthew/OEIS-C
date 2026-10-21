/*
 * A247560  a(n) = 3*a(n-1) - 4*a(n-2) with a(0) = a(1) = 1.
 * https://oeis.org/A247560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 63, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base cases ---------------------------------------------- */
    if (n < 2) return 1;                     /* a(0) = a(1) = 1 */

    /* Static cache --------------------------------------------- */
    static seq_t *cache = NULL;              /* dynamically grow array of terms */
    static idx_t cache_len = 0;              /* current number of stored terms */

    /* Initialize cache if needed */
    if (cache_len < 2) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;         /* fallback on allocation failure */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* Return already computed term */
    if (n < cache_len) return cache[n];

    /* Extend cache up to n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp) return (seq_t)0;               /* fallback on allocation failure */
    cache = tmp;

    /* Compute missing terms using recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1] - 4 * cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
