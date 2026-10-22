/*
 * A320225  a(1) = 1; a(n > 1) = Sum_{k = 1..n} Sum_{d|k, d < k} a(d).
 * https://oeis.org/A320225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = Sum_{d=1}^{n-1} a(d) * ( (n/d) - 1 )
 * Range:     u8 n <= 23, u16 n <= 109, u32 n <= 851, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from a(n) = Σ_{k=1..n} Σ_{d|k, d<k} a(d). */

#include <stdlib.h>
#include <string.h>

/* Cached values: cache[0] is unused, cache[i] holds a(i) for i ≥ 1. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* highest computed index (>=0). */
static size_t cache_cap = 0;   /* current capacity of the cache array (>=1). */

/* Grow the cache so it can index up to 'needed' (inclusive). */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();         /* allocation failure: terminate */

    /* Copy already computed entries.  cache_len+1 because index 0 is unused
       but part of the allocated block. */
    if (cache != NULL)
    {
        memcpy(new_cache, cache, (cache_len + 1) * sizeof(seq_t));
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth term of OEIS sequence A320225. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n<1 */

    /* Make sure the cache can hold indices up to n. */
    ensure_cache((size_t)n + 1);    /* indices 0..n inclusive */

    /* Initialize first term if we haven't yet. */
    if (cache_len == 0)
    {
        cache[1] = 1;
        cache_len = 1;
    }

    /* Already computed? Return it. */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t d = 1; d < i; ++d)
        {
            idx_t factor = (i / d) - 1;          /* floor(i/d) - 1 */
            sum += cache[d] * (seq_t)factor;
        }
        cache[i] = sum;
    }
    cache_len = (size_t)n;
    return cache[n];
}
