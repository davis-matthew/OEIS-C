/*
 * A242543  Number of length n+3 0..2 arrays with no four elements in a row
 *          with pattern aabb (possibly a=b) and new values 0..2 introduced in
 *          0..2 order.
 * https://oeis.org/A242543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-3) + 4*a(n-4) with a(1) = 12, a(2) = 32,
 *            a(3) = 88, a(4) = 242
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache: 1‑indexed array, cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* highest n that has been computed */
    static size_t cap = 0;     /* allocated capacity (including dummy 0) */

    /* Ensure capacity up to n */
    size_t needed = (size_t)n + 1;            /* indices 0..n inclusive */
    if (needed > cap) {
        /* Grow: at least 5 slots to hold the initial values */
        size_t newcap = (cap == 0) ? 5 : cap * 2;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise first four terms once */
    if (cached == 0) {
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)88;
        cache[4] = (seq_t)242;
        cached   = 4;
    }

    /* Compute missing terms up to n */
    if (n > cached) {
        for (idx_t i = cached + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     + 4 * cache[i - 3]
                     + 4 * cache[i - 4];
        }
        cached = n;
    }

    return cache[n];
}
