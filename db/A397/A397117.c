/*
 * A397117  Size of clusters of star triples of Family B in the Spectre
 *          aperiodic monotile tiling.
 * https://oeis.org/A397117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + a(n-3) + 12 for n > 3, with
 *            a(1)=1, a(2)=8, a(3)=50
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A397117. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Any n <= 0 is outside the official definition; return 0. */
    if (n == 0) return 0;

    /* Static cache for memoization. 1‑based indexing. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_last = 0;  /* largest n computed */

    /* Helper to enlarge the cache. */
    if (cache == NULL) {
        /* Allocate space for indices 0..3 (index 0 unused). */
        cache_cap = 4;
        cache = (seq_t*)malloc((cache_cap) * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        /* Base values.  cache[0] unused. */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)50;
        cache_last = 3;
    } else if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = 3*cache[i-1] + 15*cache[i-2] + cache[i-3] + (seq_t)12;
        }
        cache_last = n;
    }

    return cache[n];
}
