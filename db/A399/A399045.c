/*
 * A399045  Total number of connected components in all induced subgraphs of
 *          the n X 2 rook graph.
 * https://oeis.org/A399045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 26*a(n-2) + 24*a(n-3), with * a(0)=0, a(1)=3,
 *            a(2)=17. This is equivalent to * a(n) = 4^n + 3^n - 2*2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static idx_t  cache_max  = (idx_t)-1;   /* sentinel for uninitialized cache */
    static size_t cache_cap  = 0;           /* allocated capacity of the cache */

    /* Initialize the cache with the first three terms. */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                 /* allocation failed – return 0 as safe value */
            return 0;
        cache[0] = 0;                /* a(0) */
        cache[1] = 3;                /* a(1) */
        cache[2] = 17;               /* a(2) */
        cache_max = 2;
    }

    /* Return the value if it is already computed. */
    if (n <= cache_max)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                 /* allocation failed – return last known value */
            return cache[cache_max];
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 0;
        } else if (i == 1) {
            val = 3;
        } else if (i == 2) {
            val = 17;
        } else {
            /* a(i) = 9*a(i-1) - 26*a(i-2) + 24*a(i-3) */
            seq_t term1 = cache[i - 1] * (seq_t)9;
            seq_t term2 = cache[i - 3] * (seq_t)24;
            seq_t sum   = term1 + term2;
            seq_t sub   = cache[i - 2] * (seq_t)26;
            val = sum - sub;               /* guaranteed non‑negative due to recurrence */
        }
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
