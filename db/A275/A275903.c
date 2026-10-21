/*
 * A275903  Expansion of (1+4*x^2) / (1-5*x+4*x^2-4*x^3).
 * https://oeis.org/A275903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 4*a(n-3) for n > 2; a(0) = 1, a(1)
 *            = 5, a(2) = 25
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* cache array (0‑index) */
    static size_t cap   = 0;         /* allocated size of cache */
    static idx_t max_computed = 2;   /* highest n for which cache[i] is valid */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        ensure_capacity(3, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 5;   /* a(1) */
        cache[2] = 25;  /* a(2) */
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        /* request capacity at least n+1 */
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* if already computed, return immediately */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 4 * cache[i-2] + 4 * cache[i-3];
    }
    max_computed = n;

    return cache[n];
}
