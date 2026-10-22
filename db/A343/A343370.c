/*
 * A343370  a(1) = 1; a(n) = Sum_{d|n, d < n} (-1)^d * a(d).
 * https://oeis.org/A343370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = Σ_{d|n, d<n} (-1)^d * a(d)
 * Range:     i8 n <= 191, i16 n <= 8639, i32 n <= 9758, i64 n <= 9758,
 *            i128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoizes computed values in a static array that grows
   as needed. It works for any seq_t type (built‑in or arbitrary‑precision)
   and any idx_t type that behaves like uint64_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 1.  cache[0] is unused. */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* largest index already stored */

    if (n == 0)                /* offset starts at 1 */
        return 0;

    /* Ensure cache[1] is ready. */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1));
        if (!cache) abort();
        cache[0] = (seq_t)0;    /* unused */
        cache[1] = (seq_t)1;    /* a(1) = 1 */
    }

    /* Extend cache up to n, if necessary. */
    if (n > cache_cap)
    {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!new_cache) abort();
        cache = new_cache;

        for (idx_t i = cache_cap + 1; i <= n; ++i)
        {
            /* Compute a(i) using divisors of i. */
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= i; ++d)
            {
                if (i % d == 0)
                {
                    /* d is a divisor */
                    if (d < i)
                    {
                        seq_t sign = (d & (idx_t)1) ? (seq_t)-1 : (seq_t)1;
                        sum += sign * cache[d];
                    }
                    idx_t q = i / d;          /* complementary divisor */
                    if (q != d && q < i)
                    {
                        seq_t sign = (q & (idx_t)1) ? (seq_t)-1 : (seq_t)1;
                        sum += sign * cache[q];
                    }
                }
            }
            cache[i] = sum;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
