/*
 * A343390  a(n) = 1 + Product_{d|n, d < n} a(d).
 * https://oeis.org/A343390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Prod_{d|n, d<n} a(d) (OEIS A343390)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a naive O(n^2) divisor enumeration,
   suitable for n up to about 10000. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Static cache to store values up to the largest n requested so far.
   cache[i] holds a(i+1).  max_cached records the number of cached values. */
static seq_t *cache      = (seq_t*)NULL;
static size_t cache_cap  = 0;
static idx_t  max_cached = 0;

/* Ensure the cache array has space for at least `needed` entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed * 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                          /* out of memory – abort */
    if (cache)
    {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Return a(n),  n >= 1.  The function is safe for repeated random calls
   because it memoises all computed values. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined, but harmless */

    /* Expand cache if necessary. */
    ensure_capacity((size_t)n);

    /* Initialise first term if cache is empty. */
    if (max_cached == 0)
    {
        cache[0] = (seq_t)2;                /* a(1) = 2 */
        max_cached = 1;
    }

    /* Compute any values that have not yet been cached. */
    for (idx_t i = max_cached; i < n; ++i)
    {
        idx_t idx = i + 1;                  /* actual n for this entry */
        seq_t prod = (seq_t)1;
        for (idx_t d = 1; d < idx; ++d)
        {
            if (idx % d == 0)
                prod *= cache[d - 1];
        }
        cache[i] = prod + (seq_t)1;         /* a(n) = 1 + product of proper divisors */
    }
    max_cached = n;

    return cache[n - 1];                    /* return cached value */
}
