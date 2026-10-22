/*
 * A302076  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + 4*a(n-3) for n ≥ 4 with initial
 *            values a(1)=3, a(2)=10, a(3)=33
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* ensure base cases are present */
    if (cached_n < 3) {
        idx_t new_size = 3;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache[0] = (seq_t)3;  /* a(1) */
        cache[1] = (seq_t)10; /* a(2) */
        cache[2] = (seq_t)33; /* a(3) */
        cached_n = 3;
    }

    /* extend cache if necessary */
    if (n > cached_n) {
        idx_t i;
        /* Grow the cache to size n */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));

        for (i = cached_n; i < n; ++i) { /* i is zero‑based index of a(i+1) */
            /* recurrence a(i+1) = a(i) + 7*a(i-1) + 4*a(i-2) */
            cache[i] = cache[i-1] + 7 * cache[i-2] + 4 * cache[i-3];
        }
        cached_n = n;
    }

    return cache[n-1];
}
