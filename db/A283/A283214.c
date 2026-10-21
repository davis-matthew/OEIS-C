/*
 * A283214  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 597", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-2) + a(n-4) - 100*a(n-6) (n>5) a(0)=1, a(1)=10,
 *            a(2)=100, a(3)=1011, a(4)=10001, a(5)=101110 (Recurrence was
 *            conjectured by Colin Barker and verified)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* holds the computed terms   */
    static size_t cache_cap  = 0;      /* allocated array capacity   */
    static size_t cache_len  = 0;      /* number of terms actually computed */

    /* Ensure the cache can address the requested index */
    if (n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* Initialise base cases 0..5 if not yet stored */
    if (cache_len <= 5) {
        static const seq_t base_vals[6] = {
            (seq_t)1,      /* a(0)   */
            (seq_t)10,     /* a(1)   */
            (seq_t)100,    /* a(2)   */
            (seq_t)1011,   /* a(3)   */
            (seq_t)10001,  /* a(4)   */
            (seq_t)101110  /* a(5)   */
        };

        /* Fill only the missing entries up to n (or 5) */
        idx_t last = n < 5 ? n : 5;
        for (idx_t i = cache_len; i <= last; ++i)
            cache[i] = base_vals[i];
    }

    /* Compute all missing values using the recurrence */
    idx_t start = cache_len;
    if (start < 6)   /* recurrence needs terms up to a(5) */
        start = 6;

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)100 * cache[i-2]   /* 100*a(n-2)   */
                 + cache[i-4]               /* + a(n-4)      */
                 - (seq_t)100 * cache[i-6]; /* -100*a(n-6)   */
    }

    /* Update the length of the computed part of the cache */
    cache_len = (size_t)n + 1;

    return cache[n];
}
