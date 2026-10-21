/*
 * A288465  a(n) = 2*a(n-1) - a(n-4), where a(0) = 2, a(1) = 4, a(2) = 6, a(3)
 *          = 10.
 * https://oeis.org/A288465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-4)
   a(0) = 2, a(1) = 4, a(2) = 6, a(3) = 10 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated slots */
    static idx_t next_compute = 0;          /* next index to be computed */

    /* Initialise the first four values on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)10;
        next_compute = 4;
    }

    /* Grow the cache if necessary to hold indices 0..n */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    while (next_compute <= n) {
        idx_t i = next_compute;
        cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 4];
        ++next_compute;
    }

    return cache[n];
}
