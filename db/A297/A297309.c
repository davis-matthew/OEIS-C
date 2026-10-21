/*
 * A297309  Number of n X 3 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 11*a(n-2) + 3*a(n-3) - 6*a(n-4) * with initial
 *            values a(1) = 4, a(2) = 23, a(3) = 117, a(4) = 609
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* Ensure cache is initialized with the first four values */
    if (!cache) {
        /* Allocate space for indices 0..4 (index 0 unused) */
        size_t init_cap = 5; /* 0..4 inclusive */
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, unlikely in tests */

        cache[1] = (seq_t)4;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)117;
        cache[4] = (seq_t)609;
        cached_size = 4;
    }

    /* Extend cache if necessary */
    if ((idx_t)n > cached_size) {
        size_t new_cap = (size_t)n + 1; /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, unlikely */

        cache = new_cache;

        /* Compute new terms using the recurrence */
        for (idx_t i = cached_size + 1; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)4;
            else if (i == 2) cache[i] = (seq_t)23;
            else if (i == 3) cache[i] = (seq_t)117;
            else if (i == 4) cache[i] = (seq_t)609;
            else
                cache[i] = (seq_t)3 * cache[i - 1]
                         + (seq_t)11 * cache[i - 2]
                         + (seq_t)3 * cache[i - 3]
                         - (seq_t)6 * cache[i - 4];
        }

        cached_size = n;
    }

    /* Return the requested term */
    return cache[n];
}
