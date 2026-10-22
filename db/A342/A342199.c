/*
 * A342199  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k)^5 * a(k-1).
 * https://oeis.org/A342199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(n,k)^5 * a(k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"  /* for binomial() */

static seq_t *cache = NULL;          /* dynamic array of a(n) values   */
static size_t cache_cap = 0;         /* current capacity of cache      */

/* ensure the cache can hold values up to index n (inclusive) */
static void assure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory */
        if (cache)
        {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* initialise the new part with zeros – not strictly needed because
           we will overwrite all indices up to n, but it keeps the array
           clean for any debugging. */
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* compute a(n) with memoisation – works for calls in any order */
seq_t a(idx_t n)
{
    assure_cache(n);
    if (n == 0)
    {
        if (cache[0] == 0) cache[0] = (seq_t)1;
        return cache[0];
    }

    /* if already computed, return the cached value */
    if (cache[n] != 0) return cache[n];

    /* compute all values from the last cached index up to n */
    idx_t start = 1;
    /* find first not computed index */
    for (idx_t i = 0; i <= n; ++i)
        if (cache[i] == 0) { start = i; break; }

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
        {
            seq_t b = binomial(i, k);        /* binom(i,k) as seq_t      */
            /* compute b^5 */
            seq_t b2 = b * b;
            seq_t b3 = b2 * b;
            seq_t b4 = b2 * b2;
            seq_t b5 = b4 * b;
            seq_t term = b5 * cache[k - 1];
            sum += term;
        }
        cache[i] = sum;
    }

    return cache[n];
}
