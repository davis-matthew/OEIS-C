/*
 * A318372  a(1) = 1; a(n+1) = Sum_{d|n} d*a(d).
 * https://oeis.org/A318372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Sum_{d|n} d*a(d) for n >= 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free, abort */

seq_t a(idx_t n)
{
    /* cache[0] unused; cache[1] = a(1) = 1 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (indices 0..cap-1) */
    static size_t upto  = 0;   /* highest index computed */

    if (n < 1)
        return (seq_t)0; /* undefined, but harmless */

    /* Grow storage if necessary */
    if (cap <= (size_t)n)
    {
        size_t newcap = (cap == 0 ? 1 : cap * 2);
        if (newcap <= (size_t)n)
            newcap = (size_t)n + 1;
        /* realloc may return NULL on failure */
        void *tmpv = realloc(cache, newcap * sizeof(seq_t));
        if (tmpv == NULL)
            abort();
        cache = (seq_t *)tmpv;
        cap   = newcap;
    }

    /* Compute fresh values up to n */
    if (upto < (size_t)n)
    {
        for (size_t i = upto + 1; i <= (size_t)n; ++i)
        {
            if (i == 1)
            {
                cache[i] = (seq_t)1;
                continue;
            }
            idx_t m = (idx_t)i - 1;  /* we need all divisors of i-1 */
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= m; ++d)
            {
                if (m % d == 0)
                {
                    seq_t mult = (seq_t)d * cache[d];
                    sum += mult;
                    idx_t d2 = m / d;
                    if (d2 != d)
                    {
                        seq_t mult2 = (seq_t)d2 * cache[d2];
                        sum += mult2;
                    }
                }
            }
            cache[i] = sum;
        }
        upto = (size_t)n;
    }

    return cache[n];
}
