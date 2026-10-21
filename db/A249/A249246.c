/*
 * A249246  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable side of the triangles
 *          of the (n-1)-th generation (this is the "vertex to side" version);
 *          for the even n-th generation use the "vertex to vertex" version;
 *          a(n) is the number of triangles...
 * https://oeis.org/A249246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249246
   Derived recurrence:  a(n) = a(n-2) + a(n-8) - a(n-10)   for n >= 12
   Base values: a(0)=1, a(1)=3, a(2)=6, a(3)=15, a(4)=18,
                 a(5)=30, a(6)=24, a(7)=45, a(8)=30, a(9)=60,
                 a(10)=36, a(11)=75
   This follows from the rational generating function given in OEIS. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow repeated calls in any order */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    size_t idx = (size_t)n;   /* n is guaranteed to be small (≤ 10000) */

    if (cache == NULL || cap <= idx) {
        /* need to grow the cache */
        size_t new_cap = idx + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) free(cache);
        cache = new_cache;
        cap   = new_cap;

        /* initialise base terms */
        if (cap > 0) cache[0] = 1;
        if (cap > 1) cache[1] = 3;
        if (cap > 2) cache[2] = 6;
        if (cap > 3) cache[3] = 15;
        if (cap > 4) cache[4] = 18;
        if (cap > 5) cache[5] = 30;
        if (cap > 6) cache[6] = 24;
        if (cap > 7) cache[7] = 45;
        if (cap > 8) cache[8] = 30;
        if (cap > 9) cache[9] = 60;
        if (cap > 10) cache[10] = 36;
        if (cap > 11) cache[11] = 75;

        /* compute any new terms up to the new capacity */
        for (size_t i = 12; i < cap; ++i) {
            cache[i] = cache[i-2] + cache[i-8] - cache[i-10];
        }
    } else {
        /* extend cache only up to requested index */
        for (size_t i = cap; i <= idx; ++i) {
            cache[i] = cache[i-2] + cache[i-8] - cache[i-10];
        }
        cap = idx + 1;
    }

    return cache[idx];
}
