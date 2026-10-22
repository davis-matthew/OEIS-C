/*
 * A387768  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*k+1,2*n-4*k+1).
 * https://oeis.org/A387768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387768
   Formula: a(n) = Sum_{k=0..⌊n/2⌋} 2^k * binomial(2k+1, 2n-4k+1)
   Equivalent recurrence (for n ≥ 6):
     a(n) = 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 8*a(n-5) - 4*a(n-6)
   Initial values:
     a(0)=1, a(1)=0, a(2)=6, a(3)=2, a(4)=20, a(5)=40
   The function below memoises results so that calls in any order are fast. */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with known base values */
    if (cache_size == 0)
    {
        cache_size = 6;                     /* indices 0..5 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;               /* memory failure – result irrelevant */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 6;
        cache[3] = 2;
        cache[4] = 20;
        cache[5] = 40;
    }

    /* Return a cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to contain indices up to n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!new_cache) return 0;               /* memory failure – result irrelevant */
    cache = new_cache;

    /* Compute new terms, using the recurrence for n ≥ 6 */
    for (idx_t i = old_size; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)          val = 1;
        else if (i == 1)     val = 0;
        else if (i == 2)     val = 6;
        else if (i == 3)     val = 2;
        else if (i == 4)     val = 20;
        else if (i == 5)     val = 40;
        else
        {
            val = cache[i-2] * 4u;        /* 4*a(n-2) */
            val += cache[i-3] * 4u;       /* +4*a(n-3) */
            val -= cache[i-4] * 4u;       /* -4*a(n-4) */
            val += cache[i-5] * 8u;       /* +8*a(n-5) */
            val -= cache[i-6] * 4u;       /* -4*a(n-6) */
        }
        cache[i] = val;
    }

    return cache[n];
}
