/*
 * A301689  Partial sums of A301688.
 * https://oeis.org/A301689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 14, a(3) = 26, a(4) = 43, * a(5) =
 *            65, a(6) = 89, a(7) = 119 * a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4)
 *            - a(n-6) + a(n-7) for n > 6 * (partial sums of A301688)
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;      /* array that holds computed values */
static size_t cache_cap = 0;     /* allocated size of cache */
static size_t max_comp = 0;      /* highest index that has been fully computed */
static int initialized = 0;     /* flag indicating whether base values are loaded */

seq_t a(idx_t n)
{
    /* initialise cache with the first 8 terms if not yet done */
    if (!initialized) {
        /* we need indices up to 7 */
        ensure_cache(7, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)43;
        cache[5] = (seq_t)65;
        cache[6] = (seq_t)89;
        cache[7] = (seq_t)119;
        max_comp = 7;
        initialized = 1;
        /* cache_cap is at least 8 after ensure_cache */
    }

    /* if the requested index is beyond what we have computed,
       extend the cache and compute the missing values */
    if ((size_t)n > max_comp) {
        ensure_cache((idx_t)n, &cache, &cache_cap);
        for (size_t i = max_comp + 1; i <= (size_t)n; ++i) {
            /* recurrence calculation */
            cache[i] = cache[i - 1]
                       + (seq_t)2 * cache[i - 3]
                       - (seq_t)2 * cache[i - 4]
                       - cache[i - 6]
                       + cache[i - 7];
        }
        max_comp = (size_t)n;
    }

    /* return the requested value */
    return cache[(size_t)n];
}
