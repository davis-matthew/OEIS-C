/*
 * A307994  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 + x^4 * (1 + A(x) +
 *          A(x^2) + A(x^3) + ...).
 * https://oeis.org/A307994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307994
   Recurrence:  a(1) = a(2) = a(3) = a(4) = 1,
                a(n+4) = Σ_{d|n} a(d)   for n ≥ 1        */

#include <stddef.h>          /* size_t */
#include "../utils/ensure_cache.h"   /* ensure_cache */

static seq_t *cache = NULL;       /* a(1) … a(current_max) */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t cache_size = 0;      /* largest n for which values are known */

seq_t a(idx_t n)
{
    /* Handle the initial four terms directly. */
    if (n <= 4)
        return 1;

    /* Place a(1)…a(4) into the cache if we have not yet done so. */
    if (cache == NULL) {
        /* Reserve space for at least the first 4 elements. */
        ensure_cache(4, &cache, &cache_cap);
        cache[0] = cache[1] = cache[2] = cache[3] = 1;
        cache_size = 4;
    }

    /* Expand the cache to hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t m   = i - 4;          /* argument for divisor sum   */
        seq_t sum = 0;

        /* Sum a(d) for all divisors d of m. */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                /* a(d) */
                sum += cache[d - 1];
                /* a(m/d) if distinct from d */
                if (d != m / d)
                    sum += cache[(m / d) - 1];
            }
        }
        cache[i - 1] = sum;
    }

    cache_size = n;   /* mark all values up to n as computed */
    return cache[n - 1];
}
