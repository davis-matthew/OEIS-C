/*
 * A386380  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/6)} a(6*k) * a(n-1-6*k).
 * https://oeis.org/A386380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/6)} a(6*k) * a(n-1-6*k), a(0)=1
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoisation with static cache.  The ensure_cache helper guarantees
   the cache is large enough and zero‑initialised for each entry. */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;      /* cache[0..] holds computed values      */
static size_t cache_cap = 0;     /* current capacity of cache array       */

/* Recursive helper that uses memoisation */
static seq_t compute(idx_t n)
{
    /* Make sure cache is allocated */
    ensure_cache(n, &cache, &cache_cap);

    /* Base case: a(0) = 1 */
    if (n == 0)
    {
        if (cache[0] == 0)   /* initialise the first element lazily */
            cache[0] = 1;
        return cache[0];
    }

    /* Return cached value if present */
    if (cache[n] != 0)
        return cache[n];

    /* Compute a(n) using the recurrence */
    seq_t sum = 0;
    idx_t limit = (n - 1) / 6;
    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t i = 6 * k;
        idx_t j = n - 1 - 6 * k;
        sum += compute(i) * compute(j);
    }
    cache[n] = sum;
    return sum;
}

/* Public API required by the problem statement */
seq_t a(idx_t n)
{
    return compute(n);
}
