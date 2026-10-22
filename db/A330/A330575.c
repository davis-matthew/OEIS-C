/*
 * A330575  a(n) = n + Sum_{d|n and d<n} a(d) for n>1; a(1) = 1.
 * https://oeis.org/A330575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Sum_{d|n, d<n} a(d); a(1) = 1
 * Range:     u8 n <= 47, u16 n <= 2159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute term a(n) from OEIS A330575.
   Uses a static memoized cache so that calls in any order remain
   efficient.  The cache is grown incrementally to cover the largest
   index requested so far.  The implementation is fully generic
   with respect to the integer type seq_t (fixed‑width or arbitrary
   precision) and the index type idx_t. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    /* The offset is 1.  For n = 0 the caller is out of range. */
    if (n == 0) return 0;

    /* Grow the cache if the requested index is larger than we have. */
    if (n > cached_max)
    {
        idx_t new_cap = n + 1;                  /* indices 0 … n */
        seq_t *new_cache =
            (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;               /* allocation failure */
        cache = new_cache;

        /* Initialise the first term if we are starting from scratch. */
        if (cached_max == 0)
        {
            cache[1] = (seq_t)1;                /* a(1) = 1 */
            cached_max = 1;
        }

        /* Compute all new terms in increasing order. */
        for (idx_t i = cached_max + 1; i <= n; ++i)
        {
            seq_t sum = 0;

            /* Sum over all proper divisors of i. */
            for (idx_t d = 1; d * d <= i; ++d)
            {
                if (i % d == 0)
                {
                    if (d < i) sum += cache[d];

                    idx_t other = i / d;
                    if (other != d && other < i) sum += cache[other];
                }
            }
            cache[i] = (seq_t)i + sum;
        }
        cached_max = n;
    }

    return cache[n];
}
