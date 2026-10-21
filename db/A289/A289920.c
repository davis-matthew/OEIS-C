/*
 * A289920  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) - a(n-6) for n ≥ 7
 *            Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=12,
 *            a(5)=22, a(6)=42
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* allocated length of cache */
    static idx_t last_computed = 0;      /* highest index computed so far */
    static int initialized = 0;          /* flag for first-time initialization */

    /* constants for the first seven terms */
    static const seq_t init[7] = {1, 2, 3, 6, 12, 22, 42};

    /* grow cache if needed */
    if (n >= cache_len)
    {
        idx_t new_len = cache_len > 0 ? cache_len << 1 : 1;
        while (new_len <= n)
            new_len <<= 1;

        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* first call: initialise first seven terms up to min(n,6) */
    if (!initialized)
    {
        idx_t upto = n < 6 ? n : 6;
        for (idx_t i = 0; i <= upto; ++i)
            cache[i] = init[i];
        last_computed = upto;
        initialized = 1;
    }

    /* compute further terms if required */
    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            if (i <= 6)
            {
                cache[i] = init[i];
            }
            else
            {
                /* recurrence: a(n) = a(n-1) + a(n-2) + 2*a(n-3) - a(n-4) - a(n-6) */
                cache[i] = cache[i - 1] + cache[i - 2]
                           + (cache[i - 3] * (seq_t)2)
                           - cache[i - 4] - cache[i - 6];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
