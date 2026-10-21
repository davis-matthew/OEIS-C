/*
 * A295721  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = 2, a(2) = 3, a(3) = 4.
 * https://oeis.org/A295721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4); Initial values:
 *            a(0) = -1, a(1) = 2, a(2) = 3, a(3) = 4
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 42, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache with the first four terms if it does not yet exist */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache_len = 4;
    }

    /* If we already have the value return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache to hold up to term n */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    /* Fill the newly allocated entries using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] + 3 * cache[i - 2]
                   - 2 * cache[i - 3] - 2 * cache[i - 4];
    }
    cache_len = n + 1;

    return cache[n];
}
