/*
 * A386396  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/7)} a(7*k) * a(n-1-7*k).
 * https://oeis.org/A386396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..⌊(n‑1)/7⌋} a(7*k) * a(n‑1‑7*k)
 * Range:     u8 n <= 22, u16 n <= 36, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>


#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;   /* greatest index that has been computed */

    /* Make sure the cache is large enough for indices 0 … n. */
    ensure_cache(n + (idx_t)1, &cache, &cache_cap);

    /* Initialise the base case if it has not yet been set. */
    if (cache_last == 0) {
        cache[0] = (seq_t)1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_last + (idx_t)1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* k runs from 0 to floor((i‑1)/7) */
        idx_t maxk = (i >= 1) ? (i - (idx_t)1) / (idx_t)7 : (idx_t)0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t left  = cache[7 * k];
            seq_t right = cache[i - (idx_t)1 - 7 * k];
            sum += left * right;
        }
        cache[i] = sum;
    }

    if (n > cache_last) {
        cache_last = n;
    }

    return cache[n];
}
