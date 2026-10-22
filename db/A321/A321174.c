/*
 * A321174  a(n) = -2*a(n-1) + a(n-2) + a(n-3), a(0) = -1, a(1) = -4, a(2) =
 *          5.
 * https://oeis.org/A321174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2) + a(n-3), a(0) = -1, a(1) = -4, a(2) =
 *            5
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values */
    static size_t cap     = 0;      /* allocated capacity */
    static idx_t  stored  = 0;      /* index of the first missing entry (number of values stored) */

    /* initialise cache on first call */
    if (!cache) {
        cap = 8;                                            /* arbitrary initial size */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = -1;
        cache[1] = -4;
        cache[2] =  5;
        stored = 3;
    }

    if (n < stored)
        return cache[(size_t)n];

    /* enlarge buffer if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing elements up to n */
    for (idx_t i = stored; i <= n; ++i) {
        cache[(size_t)i] = -2 * cache[(size_t)(i - 1)]
                          + cache[(size_t)(i - 2)]
                          + cache[(size_t)(i - 3)];
    }
    stored = n + 1;                 /* we now have values up to n */
    return cache[(size_t)n];
}
