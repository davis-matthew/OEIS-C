/*
 * A280222  a(n) = (-3)^(n-1)*a(n-1) + a(n-2).
 * https://oeis.org/A280222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-3)^(n-1)*a(n-1) + a(n-2) a(0) = 0, a(1) = 1 Recurrence
 *            is evaluated iteratively with memoisation
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Memoisation cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* current allocated size of cache */
    static idx_t computed_max = 0;   /* last index that has been computed */
    static seq_t pow_minus3 = 0;     /* (-3)^(k-1) for the current k */

    /* Initialise cache and base values on first call. */
    if (cache == NULL) {
        /* ensure space for indices 0 and 1 */
        ensure_cache(1, &cache, &capacity);

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed_max = 1;
        pow_minus3   = (seq_t)(-3);   /* exponent (k-1) = 1 for k = 2 */
    }

    /* If the requested index is already known, return it. */
    if (n <= computed_max)
        return cache[n];

    /* Grow the cache so that it can hold at least (n+1) elements. */
    ensure_cache(n, &cache, &capacity);

    /* Compute and store all subsequent terms up to n. */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t ak = pow_minus3 * cache[k - 1] + cache[k - 2];
        cache[k] = ak;
        pow_minus3 = pow_minus3 * (seq_t)(-3);   /* advance power for next k */
    }

    computed_max = n;
    return cache[n];
}
