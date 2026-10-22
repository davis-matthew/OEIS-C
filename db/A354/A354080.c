/*
 * A354080  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0)=1, a(1)=4, a(2)=5.
 * https://oeis.org/A354080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); a(0)=1, a(1)=4, a(2)=5
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* current allocated capacity */
    static idx_t   max   = 0;   /* highest index already computed (inclusive) */

    /* Initialisation: store the three seed values */
    if (cache == NULL) {
        /* allocate space for at least three elements */
        ensure_cache(3, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)5;
        max     = 2;
    }

    /* If the requested index is already available, return it */
    if (n <= max) {
        return cache[n];
    }

    /* Make sure we have enough space for indices up to n */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* Iteratively compute missing terms */
    for (idx_t i = max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    max = n;
    return cache[n];
}
