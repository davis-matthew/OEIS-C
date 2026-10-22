/*
 * A307204  Coordination sequence for trivalent node of type alpha'''' in the
 *          first Moore pentagonal tiling.
 * https://oeis.org/A307204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307204: Coordination sequence for trivalent node of type alpha''''
   Recurrence for n > 17:
       a(n) = 2*a(n-4) - a(n-8)
   Initial values (indices 0‑17):
       1, 3, 6, 6, 12, 18, 24, 30, 30, 33, 48, 57, 60, 60, 72, 84, 78, 81
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   max_n = 0;
    static bool    ready = false;

    /* fixed initial values */
    static const seq_t init_vals[18] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)6, (seq_t)12,
        (seq_t)18, (seq_t)24, (seq_t)30, (seq_t)30, (seq_t)33,
        (seq_t)48, (seq_t)57, (seq_t)60, (seq_t)60, (seq_t)72,
        (seq_t)84, (seq_t)78, (seq_t)81
    };

    if (!ready) {
        /* initialise cache to hold at least 18 entries */
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i)
            cache[i] = init_vals[i];
        max_n = 17;
        ready = true;
    }

    /* if requested index already cached, return it */
    if (n <= max_n)
        return cache[n];

    /* grow cache if necessary */
    size_t required = (size_t)(n + 1);
    if (required > cap) {
        size_t new_cap = cap;
        while (new_cap < required)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* compute successive terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* for i > 17, use the second-order recurrence */
        seq_t v1 = cache[i - 4];
        seq_t v2 = cache[i - 8];
        cache[i] = v1 + v1 - v2;          /* 2*v1 - v2 */
    }

    max_n = n;
    return cache[n];
}
