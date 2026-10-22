/*
 * A306603  a(n) = (2 cos(Pi/15))^n + (2 cos(7 Pi/15))^n + (2 cos(11 Pi/15))^n
 *          + (2 cos(13 Pi/15))^n.
 * https://oeis.org/A306603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n-1)) + 4*a(n-2) + 4*a(n-3) – a(n-4) Initial values:
 *            a(0)=4, a(1)=-1, a(2)=9, a(3)=-1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only the standard headers needed for memory allocation */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* pointer to computed terms   */
    static size_t  cap    = 0;      /* current allocated size      */
    static idx_t   last   = (idx_t)-1;   /* last index computed          */
    static const seq_t init[4] = { 4, -1, 9, -1 };  /* a(0)..a(3) */

    /* small n → return the constant directly */
    if (n < 4) return init[n];

    /* first allocation of the cache */
    if (cache == NULL) {
        size_t new_cap = 16;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();
        cap   = new_cap;
        /* initialise the first four terms */
        for (int i = 0; i < 4; ++i) cache[i] = init[i];
        last = 3;
    }

    /* grow the buffer if necessary (doubling keeps amortised O(1)) */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = -cache[i-1]
                       + 4 * cache[i-2]
                       + 4 * cache[i-3]
                       - cache[i-4];
        }
        last = n;
    }

    return cache[n];
}
