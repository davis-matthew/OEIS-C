/*
 * A278066  Relative of Hofstadter Q-sequence: a(1) = 5, a(2) = 2; thereafter
 *          a(n) = a(n-a(n-1)) + a(n-a(n-2)).
 * https://oeis.org/A278066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278066:
   a(1) = 5, a(2) = 2,
   a(n) = a(n - a(n-1)) + a(n - a(n-2))  for n > 2
   (with a(k) = 0 for k <= 0 by convention).          */

#include <stdlib.h>
#include <string.h>

/* Cached values: cache[0] is kept 0 for the a(k)=0 convention. */
static seq_t *cache      = NULL;   /* 0..cache_cap-1 allocated */
static size_t cache_cap  = 0;      /* current allocation size */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    /* required indices are 0..n inclusive, so size needs n+1. */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;   /* already sufficient */

    size_t new_cap = (cache_cap < 16) ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();  /* allocation failure */

    /* New memory produced by realloc is not zero‑initialized, so we do it. */
    if (new_cache != cache)
    {
        /* Zero the newly added part only. */
        size_t offset = cache_cap;
        size_t new_part = new_cap - cache_cap;
        memset(new_cache + offset, 0, new_part * sizeof(seq_t));
    }
    else
    {
        /* First allocation; zero everything. */
        memset(new_cache, 0, new_cap * sizeof(seq_t));
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* by convention */

    ensure_capacity(n);

    /* Compute all terms up to n in order, storing results in cache. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0)
            continue;                  /* already computed */

        if (i == 1)
        {
            cache[i] = (seq_t)5;
        }
        else if (i == 2)
        {
            cache[i] = (seq_t)2;
        }
        else
        {
            /* Index for a(i-a(i-1)).  If a(i-1) > i, treat as 0. */
            idx_t idx1 = 0;
            if (cache[i-1] <= (seq_t)i)
            {
                /* Safe: (i - a(i-1)) is non‑negative. */
                seq_t diff = (seq_t)i - cache[i-1];
                idx1 = (idx_t)diff;
            }

            /* Index for a(i-a(i-2)).  If a(i-2) > i, treat as 0. */
            idx_t idx2 = 0;
            if (cache[i-2] <= (seq_t)i)
            {
                seq_t diff = (seq_t)i - cache[i-2];
                idx2 = (idx_t)diff;
            }

            seq_t v1 = (idx1 > 0) ? cache[idx1] : 0;
            seq_t v2 = (idx2 > 0) ? cache[idx2] : 0;
            cache[i] = v1 + v2;
        }
    }
    return cache[n];
}
