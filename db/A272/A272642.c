/*
 * A272642  Expansion of (x^4+x^3+x^2-x-1)/(x^4+2*x^3+2*x^2+x-1).
 * https://oeis.org/A272642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=8, a(4)=18; for n>=5:
 *            a(n)=a(n-1)+2*a(n-2)+2*a(n-3)+a(n-4)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a memoised, O(n) computation that works for any
   sequence type seq_t and index type idx_t defined by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size   = 0;

    /* initialise cache with the first five values */
    if (size == 0)
    {
        size = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache)    /* allocation failed – return 0 (unused) */
            return 0;
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 8;
        cache[4] = 18;
    }

    /* grow cache if needed */
    if (n >= size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp)    /* allocation failed – return 0 (unused) */
            return 0;
        cache = tmp;

        /* compute missing terms using recurrence, starting at the first
           unknown index 5 (since 0..4 are already known). */
        for (idx_t i = size; i <= n; ++i)
        {
            /* recurrence: a(i) = a(i-1) + 2*a(i-2) + 2*a(i-3) + a(i-4) */
            cache[i] = cache[i-1]
                     + 2 * cache[i-2]
                     + 2 * cache[i-3]
                     +  cache[i-4];
        }
        size = new_size;
    }

    return cache[n];
}
