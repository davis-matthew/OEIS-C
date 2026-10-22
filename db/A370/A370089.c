/*
 * A370089  Number of dominating sets in the n-trapezohedral graph.
 * https://oeis.org/A370089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370089: Number of dominating sets in the n‑trapezohedral graph
   Recurrence (order 9):
      a(n) = 12*a(n-1) - 50*a(n-2) + 75*a(n-3) + 5*a(n-4)
             - 66*a(n-5) - 12*a(n-6) + 11*a(n-7) + 18*a(n-8) + 8*a(n-9)
   Initial values:
      a(0) = 2, a(1) = 9, a(2) = 45, a(3) = 183, a(4) = 693,
      a(5) = 2589, a(6) = 9639, a(7) = 35933, a(8) = 134373
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  last_computed = 8;  /* highest index computed */
    static int    initialized = 0;

    /* Base values up to index 8 */
    const seq_t base[9] = {
        (seq_t)2,   (seq_t)9,   (seq_t)45,  (seq_t)183,
        (seq_t)693, (seq_t)2589, (seq_t)9639,(seq_t)35933,
        (seq_t)134373
    };

    /* First invocation: allocate and initialize cache up to index 8 */
    if (!initialized) {
        cache_size = 9;  /* indices 0..8 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();  /* Out of memory */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = base[i];
        /* Remaining indices will be computed on demand */
        initialized = 1;
    }

    /* If requested index is beyond current cache, enlarge it */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
        /* Zero-fill new slots (they will be computed before use) */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* If the term is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)12 * cache[i-1]
            - (seq_t)50 * cache[i-2]
            + (seq_t)75 * cache[i-3]
            + (seq_t)5  * cache[i-4]
            - (seq_t)66 * cache[i-5]
            - (seq_t)12 * cache[i-6]
            + (seq_t)11 * cache[i-7]
            + (seq_t)18 * cache[i-8]
            + (seq_t)8  * cache[i-9];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
