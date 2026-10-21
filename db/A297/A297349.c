/*
 * A297349  Number of edge covers in the 2 X n king graph.
 * https://oeis.org/A297349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=41 a(n) = 29*a(n-1) + 36*a(n-2) - 24*a(n-3)
 *            for n >= 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for memoised sequence values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* actual allocation size (number of elements) */
    static size_t cached = 0;     /* next index still to be computed */

    /* Grow the cache if necessary.  Ensure at least 3 elements for
       the base cases a(0), a(1), a(2). */
    if ((size_t)n >= capacity) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 3) new_cap = 3;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failure */
        capacity = new_cap;
    }

    /* Initialise base cases on first entrance. */
    if (cached == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)41;
        cached = 3;
    }

    /* Compute missing terms up to n (inclusive). */
    while (cached <= (size_t)n) {
        cache[cached] =
            (seq_t)29 * cache[cached - 1] +
            (seq_t)36 * cache[cached - 2] -
            (seq_t)24 * cache[cached - 3];
        cached++;
    }

    return cache[(size_t)n];
}
