/*
 * A295091  Number of n X 2 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 3 1s.
 * https://oeis.org/A295091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295091: Number of n×2 0/1 arrays where each 1 has 1 or 3 adjacent 1s.
   Formula: a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-5)   for n>5
   Initial values:
       a(1) = 2, a(2) = 5, a(3) = 11, a(4) = 24, a(5) = 55
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for indices below the offset */
        return 0;

    static seq_t *cache   = NULL;   /* cache[1]..cache[max_n] */
    static size_t cached_n = 0;     /* largest index computed so far */

    /* Ensure the cache is large enough for index n */
    size_t required = (size_t)n;
    if (!cache || required + 1 > cached_n + 1) {
        size_t new_size = required + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* allocation failed */
            return 0;
        cache = tmp;
    }

    /* If we need to compute up to a new index */
    if (required > cached_n) {
        /* Set base values (1 for 5) within the allocated range */
        if (required >= 1) cache[1] = (seq_t)2;
        if (required >= 2) cache[2] = (seq_t)5;
        if (required >= 3) cache[3] = (seq_t)11;
        if (required >= 4) cache[4] = (seq_t)24;
        if (required >= 5) cache[5] = (seq_t)55;

        /* Compute recurrence for new indices */
        size_t start = cached_n + 1;  /* next index not yet computed */
        if (start < 6)                /* recurrence only valid for n>5 */
            start = 6;
        for (size_t i = start; i <= required; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     + (seq_t)2 * cache[i-3]
                     - cache[i-5];
        }
        cached_n = required;
    }

    return cache[required];
}
