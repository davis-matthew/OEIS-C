/*
 * A321025  a(n) = sum of a(n-4) and a(n-5), with the lowest possible initial
 *          values that will generate a sequence where a(n) is always >
 *          a(n-1): 4, 5, 6, 7 and 8.
 * https://oeis.org/A321025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5) with a(1)=4, a(2)=5, a(3)=6, a(4)=7,
 *            a(5)=8
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity (elements)   */
    static size_t   sz    = 0;   /* number of elements already filled */

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cap = 8;                                /* at least 5 slots + slack   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failure */

        cache[0] = 4;   /* a(1) */
        cache[1] = 5;   /* a(2) */
        cache[2] = 6;   /* a(3) */
        cache[3] = 7;   /* a(4) */
        cache[4] = 8;   /* a(5) */
        sz = 5;
    }

    /* If the requested term has already been computed, return it */
    if ((size_t)n <= sz)
        return cache[(size_t)n - 1];

    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;              /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all missing terms up to n */
    for (size_t i = sz; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1).  For i >= 5 (i.e. n >= 6) we use
           the recurrence a(n) = a(n-4) + a(n-5). */
        cache[i] = cache[i - 4] + cache[i - 5];
    }

    sz = (size_t)n;
    return cache[(size_t)n - 1];
}
