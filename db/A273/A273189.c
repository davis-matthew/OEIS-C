/*
 * A273189  a(n) is the third number in a triple consisting of 3 numbers,
 *          which when squared are part of a right diagonal of a magic square
 *          of squares.
 * https://oeis.org/A273189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 96 * Initial values: a(0) = 51, a(1)
 *            = 401 * * Supports any seq_t (unsigned integer or
 *            arbitrary‑precision) and * any idx_t provided by the build
 *            system. The calculation uses only * +, -, * operations that work
 *            with all such types, so the implementation * is valid as bo...
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

/* seq_t and idx_t are defined by the build system. */

/* Cached sequence values:  a[0] = 51, a[1] = 401 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* pointer to allocated array */
    static idx_t  cap   = 0;      /* array capacity (number of slots) */
    static idx_t  len   = 0;      /* number of terms currently stored */

    /* Initialise cache on the first call */
    if (cache == NULL) {
        /* start with space for two terms */
        cap = 2;
        len = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)51;
        cache[1] = (seq_t)401;
    }

    /* Grow the cache if necessary to reach index n */
    if (n >= cap) {
        idx_t newcap = cap;
        /* exponential growth to keep amortised realloc cost low */
        while (newcap <= n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) continue;      /* already initialised */
            if (i == 1) continue;
            /* a(i) = 6*a(i-1) - a(i-2) + 96 ;
             * works for all seq_t types because it only uses +, -, *.
             */
            seq_t val = cache[i-1] * 6 - cache[i-2] + 96;
            cache[i]  = val;
        }
        len = n + 1;
    }

    return cache[n];
}
