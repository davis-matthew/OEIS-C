/*
 * A341697  a(1) = a(2) = 1; a(n+1) = Sum_{d|n, d < n} a(n/d) * a(d).
 * https://oeis.org/A341697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n+1) = Sum_{d|n, d < n} a(n/d) * a(d) (OEIS
 *            A341697)
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache.  cache[0] is unused. */
    static seq_t *cache = NULL;       /* array of a(k) with k ≥ 0   */
    static idx_t cache_size = 0;      /* allocated size (≥ 0)       */
    static idx_t last_computed = 0;   /* largest n whose value is set */

    if (n == 0)           /* undefined by definition, return 0 safely */
        return (seq_t)0;

    /* Grow the cache if the requested n is beyond the current capacity */
    if (n >= cache_size) {
        idx_t new_size = n + 1;          /* we want indices 0..n   */
        seq_t *new_cache = alloc_cache(new_size);
        if (new_cache == NULL)
            return (seq_t)0;              /* allocation failure */

        /* copy previously computed values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;

        /* free old cache (compatible with seq_t implementation) */
        free_cache(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        if (k <= 2) {                     /* base values */
            cache[k] = (seq_t)1;
            continue;
        }

        idx_t m = k - 1;                 /* compute a(k) from a(m) */
        seq_t sum = (seq_t)0;

        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;         /* paired divisor */

                /* divisor d (always < m) */
                sum += cache[d] * cache[e];

                /* paired divisor e, if distinct from d and < m */
                if (e != d && e < m)
                    sum += cache[e] * cache[d];
            }
        }
        cache[k] = sum;
    }
    last_computed = n;
    return cache[n];
}
