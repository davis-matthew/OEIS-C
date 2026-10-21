/*
 * A280042  a(1) = 1, a(n+1) is the numerator of n-th partial fraction of the
 *          continued fraction [1; 10, 100, 1000, ..., 10^n].
 * https://oeis.org/A280042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 10^(n-1)*a(n-1) + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   ensure_cache, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"   /* ensure_cache */
#include "../utils/pow10.h"          /* pow10 (not used below, but kept for reference) */

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n <= 1)
        return (seq_t)1;

    /* Static cache of already computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Make sure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base cases once. */
    if (cache[0] == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1  (needed for the recurrence) */
        cache[1] = (seq_t)1;   /* a(1) = 1  */
    }

    /* If we already have the requested value, return it. */
    if (cache[n] != 0)
        return cache[n];

    /* Incrementally grow the cache up to n. */
    seq_t pow10_val = (seq_t)10;   /* 10^(i-1) for the first iteration i=2 */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) only if it hasn't been computed yet. */
        if (cache[i] == 0) {
            cache[i] = pow10_val * cache[i - 1] + cache[i - 2];
        }
        /* Update 10^(i) for the next round. */
        pow10_val *= (seq_t)10;
    }

    return cache[n];
}
