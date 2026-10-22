/*
 * A382732  Number of proper hypercubic bipartitions of n.
 * https://oeis.org/A382732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0, a(n) = a(2^ceil(log2(n)) - n) + 1 for n > 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple iterative computation with lazy caching.  */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of cached values  */
    static idx_t cache_size = 0;    /* current allocated size  */

    /* Make sure the cache can hold values up to 'n'.  */
    ensure_cache(n, &cache, &cache_size);

    /* Initialise base cases once.  */
    if (n >= 0) cache[0] = 0;
    if (n >= 1) cache[1] = 0;

    /* Compute values iteratively from 2 up to n.  */
    idx_t pow2 = 1; /* Current power of two (smallest >= current i) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Advance pow2 until it is at least i. */
        while (pow2 < i)
            pow2 <<= 1;

        /* The recurrence index j is always < i for i>1. */
        idx_t j = pow2 - i;
        cache[i] = cache[j] + 1;
    }

    return cache[n];
}
