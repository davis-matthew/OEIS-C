/*
 * A320063  A sequence which changes by one or zero: a(n) = a(n-1-a(a(n-1))) +
 *          a(a(a(n-1))) for n > 1, a(n) = n for n < 2.
 * https://oeis.org/A320063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1-a(a(n-1))) + a(a(a(n-1))) for n > 1, a(0) = 0, a(1)
 *            = 1
 * Range:     u8 n <= 592, u16 n <= 9863, u32 n <= 9863, u64 n <= 9863,
 *            u128 n <= 9863, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the trivial values directly. */
    if (n < 2) return (seq_t)n;

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;      /* pointer to the array of terms */
    static idx_t  cap   = 0;         /* number of elements currently allocated */

    /* First time: allocate enough space and initialise the base cases. */
    if (!cache)
    {
        cap = n + 1;                                 /* we need indices 0..n */
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;

        /* Iterate from 2 up to n, filling the cache. */
        for (idx_t i = 2; i <= n; ++i)
        {
            seq_t x    = cache[i - 1];                 /* a(i-1)      */
            seq_t ax   = cache[(size_t)x];            /* a(a(i-1))   */
            idx_t  idx = (i - 1) - (idx_t)ax;          /* i-1-a(a(i-1)) */
            seq_t first  = cache[idx];                /* a(idx)      */
            seq_t second = cache[(size_t)ax];         /* a(a(a(i-1))) */
            cache[i] = first + second;
        }
        return cache[n];
    }

    /* If the requested index is already in the cache. */
    if (n < cap)
        return cache[n];

    /* Grow the cache to accommodate index n. */
    size_t old_cap = cap;
    cap = n + 1;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));

    /* Extend the cache: compute new values from old_cap up to n. */
    for (idx_t i = old_cap; i <= n; ++i)
    {
        seq_t x    = cache[i - 1];                 /* a(i-1)      */
        seq_t ax   = cache[(size_t)x];            /* a(a(i-1))   */
        idx_t  idx = (i - 1) - (idx_t)ax;          /* i-1-a(a(i-1)) */
        seq_t first  = cache[idx];                /* a(idx)      */
        seq_t second = cache[(size_t)ax];         /* a(a(a(i-1))) */
        cache[i] = first + second;
    }

    return cache[n];
}
