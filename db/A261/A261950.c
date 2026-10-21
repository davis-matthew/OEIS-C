/*
 * A261950  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable vertex of the
 *          triangles of the (n-1)-th generation (this is the "side to vertex"
 *          version); for the even n-th generation use the "vertex to vertex"
 *          version; a(n) is the number of triangl...
 * https://oeis.org/A261950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..6) = 1,3,9,12,30,18,45 Recurrence for n>6: a(n) = a(n-2) +
 *            a(n-4) - a(n-6)
 * Range:     u8 n <= 29, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for n = 0..6 */
    static const seq_t base[7] = {1, 3, 9, 12, 30, 18, 45};

    /* Memoisation cache */
    static seq_t *cache   = NULL;
    static size_t  cap   = 0;   /* allocation capacity */
    static size_t  sz    = 0;   /* number of values cached (<= cap) */

    /* Handle small n directly */
    if (n < 7)
        return base[n];

    /* Initialise cache on first call */
    if (!cache) {
        cap = 7;
        sz  = 7;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;               /* fallback on allocation failure */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base[i];
    }

    /* Grow cache array if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 2] + cache[i - 4] - cache[i - 6];
        cache[i] = val;
    }
    sz = (size_t)n + 1;   /* updated number of cached values */

    return cache[(size_t)n];
}
