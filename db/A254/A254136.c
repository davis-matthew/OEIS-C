/*
 * A254136  Indices of pentagonal numbers (A000326) which are also centered
 *          hexagonal numbers (A003215).
 * https://oeis.org/A254136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 1154*a(n-2) - 1154*a(n-3) - a(n-4) + a(n-5)
   Initial values: a(1)=1, a(2)=73, a(3)=889, a(4)=84049, a(5)=1025713 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 1 – a(1) is defined.  Return 0 for any n < 1. */
    if (n < 1) return (seq_t)0;

    /* Static cache that persists between calls. */
    static seq_t *cache = NULL;
    static idx_t highest = 0;          /* highest index that has been computed */
    static size_t capacity = 0;        /* allocated array size (including dummy 0) */

    /* Handle small indices directly (saved from the cache because the
       cache may not have been initialized yet). */
    if (n <= 5) {
        /* Still allocate cache if it hasn't been (needed for later calls). */
        if (!cache) {
            capacity = 16;
            cache = (seq_t*)malloc(capacity * sizeof(seq_t));
            if (!cache) return (seq_t)0;
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)73;
            cache[3] = (seq_t)889;
            cache[4] = (seq_t)84049;
            cache[5] = (seq_t)1025713;
            highest = 5;
        }
        /* Return a small index without further computation. */
        return cache[n];
    }

    /* First time we need a value >5: initialise cache if necessary. */
    if (!cache) {
        /* Allocate enough space for at least indices 0..5. */
        capacity = (size_t)16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)73;
        cache[3] = (seq_t)889;
        cache[4] = (seq_t)84049;
        cache[5] = (seq_t)1025713;
        highest = 5;
    }

    /* Grow the cache if the desired index is beyond the current capacity. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t k = highest + 1; k <= n; ++k) {
        seq_t v = cache[k-1]
                + (seq_t)1154 * cache[k-2]
                - (seq_t)1154 * cache[k-3]
                - cache[k-4]
                + cache[k-5];
        cache[k] = v;
    }
    highest = n;

    return cache[n];
}
