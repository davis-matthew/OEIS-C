/*
 * A251251  Number of (n+1) X (1+1) 0..1 arrays with every 2 X 2 subblock
 *          having a single 1 or two 1s on the same edge.
 * https://oeis.org/A251251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) (n >= 4) Initial values: a(1)
 *            = 8, a(2) = 18, a(3) = 38 The sequence is offset 1. A memoised
 *            recurrence implementation with dynamic array
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 55, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure base terms are stored */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)8;
            case 2: return (seq_t)18;
            case 3: return (seq_t)38;
        }
    }

    /* initialise cache with first three terms if not yet done */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)38;
    }

    /* extend cache if needed */
    if (n > cache_size) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cache_size = n;
    }

    /* compute missing terms using recurrence */
    for (idx_t i = 3; i < n; ++i) {
        cache[i] = cache[i-1] + (seq_t)3 * cache[i-2] - cache[i-3];
    }

    return cache[n-1];
}
