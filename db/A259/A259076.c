/*
 * A259076  Powers of 80.
 * https://oeis.org/A259076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*a(n-1) (a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed powers */
    static size_t cache_cap = 0;  /* capacity of allocated array */
    static idx_t last_n = 0;      /* highest index computed so far */

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* On first call, initialise a(0) */
    if (last_n == 0) {
        cache[0] = (seq_t)1;
        last_n = 0;
    }

    /* Compute new powers up to n if needed */
    if (n > last_n) {
        for (idx_t i = last_n + 1; i <= n; ++i)
            cache[i] = cache[i - 1] * (seq_t)80;
        last_n = n;
    }

    return cache[n];
}
