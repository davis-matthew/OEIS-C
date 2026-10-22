/*
 * A322460  Sum of n-th powers of the roots of x^3 + 95*x^2 - 88*x - 1.
 * https://oeis.org/A322460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -95*a(n-1) + 88*a(n-2) + a(n-3), a(0) = 3, a(1) = -95,
 *            a(2) = 9201
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t last = (idx_t)-1;   /* highest index already computed */
    static size_t cap = 0;            /* allocated cache size */

    /* Helper: ensure cache covers indices up to n */
    if (cache == NULL) {
        /* On first call, allocate at least space for the first three terms */
        cap = (size_t)(n < 3 ? 3 : n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        /* initialise first three terms */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-95;
        cache[2] = (seq_t)9201;
        last = 2;
    } else if ((size_t)(n + 1) > cap) {
        /* Grow the cache to fit n */
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* If the requested value is already computed, return it */
    if (n <= last) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* All arithmetic uses seq_t, so the implementation works
           for built‑in integer types as well as for arbitrary‑precision classes.   */
        cache[i] = -95 * cache[i - 1] + 88 * cache[i - 2] + cache[i - 3];
    }
    last = n;

    return cache[n];
}
