/*
 * A364161  G.f. satisfies A(x) = 1 + x*A(x)^2/(1 - x^3*A(x)).
 * https://oeis.org/A364161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364161
 *
 * Recurrence (valid for n ≥ 8):
 *     (n+1)*a(n) = (4n-2)*a(n-1) - (n+1)*a(n-2) + (6n-9)*a(n-3)
 *                  + (2n-7)*a(n-5) - (n-8)*a(n-6) - (n-8)*a(n-8)
 *
 * Initial values:
 *     a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=15,
 *     a(5)=47, a(6)=153, a(7)=514
 *
 * The function below computes a(n) for any n ≥ 0 using a static
 * memoised cache and the above recurrence.  All operations use the
 * integer types supplied by the build system: seq_t for sequence
 * values and idx_t for indices.
 */

#include <stddef.h>      /* for size_t, NULL */
#include <stdbool.h>     /* for bool */
#include "../utils/ensure_cache.h"  /* for ensure_cache() */

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Static cache for memoisation: cache[i] == a(i) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity in entries */
    static idx_t  cache_last = 0;  /* highest index that has been computed */

    /* Initialise the cache with the first eight known values. */
    if (cache == NULL) {
        /* Allocate space for indices 0..7 (8 entries). */
        ensure_cache(7, &cache, &cache_cap);

        const seq_t init[8] = {1, 1, 2, 5, 15, 47, 153, 514};
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];

        cache_last = 7;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_last)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* Compute values from cache_last+1 up to n inclusive. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* Positive contributions. */
        seq_t pos = ((seq_t)(4 * i - 2)) * cache[i - 1]
                  + ((seq_t)(6 * i - 9)) * cache[i - 3]
                  + ((seq_t)(2 * i - 7)) * cache[i - 5];

        /* Negative contributions. */
        seq_t neg = ((seq_t)(i + 1)) * cache[i - 2]
                  + ((seq_t)(i - 8)) * cache[i - 6]
                  + ((seq_t)(i - 8)) * cache[i - 8];

        /* a(i) = (pos - neg) / (i+1)  -- division is exact. */
        cache[i] = (pos - neg) / ((seq_t)(i + 1));
    }

    cache_last = n;
    return cache[n];
}
