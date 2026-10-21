/*
 * A256019  a(n) = Sum_{i=1..n-1} (i^3 * a(i)), a(1)=1.
 * https://oeis.org/A256019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=2}^{n-1} (i^3+1) , a(1)=1 , a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoization; a[0] stores a(1), a[1] stores a(2), etc. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* ensure cache contains at least a(n).  Compute missing entries. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;

    idx_t new_cap = n;
    seq_t *new_cache = NULL;

    if (cache == NULL)
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    if (new_cache == NULL)
    {
        /* Allocation failure; abort.  The build environment guarantees enough memory. */
        abort();
    }

    /* Compute new entries from cache_cap+1 up to n */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
    {
        if (i == 1 || i == 2)
        {
            new_cache[i - 1] = (seq_t)1;
        }
        else
        {
            /* a(i) = a(i-1) * ((i-1)^3 + 1) */
            seq_t prev = new_cache[i - 2];                    /* a(i-1) */
            seq_t base = (seq_t)(i - 1);
            seq_t term = base * base * base;                  /* (i-1)^3 */
            term += (seq_t)1;                                 /* +1 */
            new_cache[i - 1] = prev * term;                   /* a(i) */
        }
    }

    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
    {
        /* Undefined for n < 1; return 0 as a safety fallback. */
        return (seq_t)0;
    }

    /* Ensure cache has value up to a(n) and return it. */
    ensure_cache(n);
    return cache[n - 1];
}
