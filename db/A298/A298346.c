/*
 * A298346  a(n) = a(n-1) + a(n-2) + 2 a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a([n/2]), a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t size    = 0;   /* Number of terms currently cached */
    static size_t capacity = 0;  /* Allocated size of the cache array */

    /* First call: create cache with enough room for the requested n */
    if (!cache) {
        capacity = (size_t)(n + 1);
        if (capacity < 3) capacity = 3;                 /* at least a(0..2) */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 1;                                   /* a(0) */
        cache[1] = 1;                                   /* a(1) */
        cache[2] = 1;                                   /* a(2) */
        size    = 3;
    }

    /* If already computed, return immediately */
    if ((size_t)n < size)
        return cache[n];

    /* Grow the array if necessary */
    if ((size_t)n + 1 > capacity) {
        capacity = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* Compute missing terms in increasing order */
    for (size_t idx = size; idx <= (size_t)n; ++idx) {
        if (idx <= 2) continue;                        /* base cases already set */

        seq_t half = cache[idx / 2];                    /* a([idx/2]) */
        cache[idx] = cache[idx - 1] + cache[idx - 2] + half + half;
    }

    size = (size_t)n + 1;
    return cache[n];
}
