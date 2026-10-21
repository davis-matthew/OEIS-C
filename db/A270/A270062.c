/*
 * A270062  Number of tilings of a 2 X n rectangle using monominoes and
 *          trominoes of any shape.
 * https://oeis.org/A270062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) +5*(a(n-2)+a(n-3)) + a(n-4) - a(n-5) - a(n-6)
 *            Initial values: a(0)=1, a(1)=1, a(2)=5, a(3)=14, a(4)=45,
 *            a(5)=140
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n < 6) {
        static const seq_t base[6] = {
            (seq_t)1,
            (seq_t)1,
            (seq_t)5,
            (seq_t)14,
            (seq_t)45,
            (seq_t)140
        };
        return base[n];
    }

    /* Static cache for memoization; grows with the maximum n seen. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Initialise base entries once */
    if (cache == NULL) {
        cache = (seq_t*)calloc(6, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)45;
        cache[5] = (seq_t)140;
        cap     = 6;
    }

    /* Extend the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */
        cache = tmp;
        /* Compute new terms */
        for (size_t i = cap; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)5 * (cache[i-2] + cache[i-3])
                     + cache[i-4]
                     - cache[i-5]
                     - cache[i-6];
        }
        cap = new_cap;
    }
    return cache[n];
}
