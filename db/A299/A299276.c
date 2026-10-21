/*
 * A299276  Partial sums of A008137.
 * https://oeis.org/A299276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) +
 *            3*a(n-7) - a(n-8) for n>7; a(0)=1, a(1)=5, a(2)=14, a(3)=31,
 *            a(4)=59, a(5)=101, a(6)=161, a(7)=242
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initialise cache with the first 8 terms */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* out of memory guard */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)59;
        cache[5] = (seq_t)101;
        cache[6] = (seq_t)161;
        cache[7] = (seq_t)242;
    }

    /* extend cache and compute missing terms if necessary */
    if ((size_t)n >= cap) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out of memory guard */
        cache = tmp;
        cap = new_cap;

        for (idx_t i = old_cap; i <= n; ++i) {
            /* recurrence holds for all i >= 8 */
            cache[(size_t)i] =
                  3 * cache[(size_t)(i-1)]
                - 3 * cache[(size_t)(i-2)]
                +  cache[(size_t)(i-3)]
                +  cache[(size_t)(i-5)]
                - 3 * cache[(size_t)(i-6)]
                + 3 * cache[(size_t)(i-7)]
                -  cache[(size_t)(i-8)];
        }
    }

    return cache[(size_t)n];
}
