/*
 * A353547  Expansion of e.g.f. -log(1-3*x) * exp(x)/3.
 * https://oeis.org/A353547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-2)*a(n-1) - 3*(n-1)*a(n-2) + 1, a(0) = 0, a(1) = 1.
 *            (OEIS A353547)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for any n >= 0.  Results are memoized for efficiency. */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;   /* array of a[0]..a[cap-1] */
    static idx_t cap   = 0;       /* capacity of 'cache' */
    static idx_t maxc  = 0;       /* largest index computed so far */

    /* Allocate or grow the cache to contain indices up to n */
    if (cap <= n)
    {
        idx_t need = n + 1;                     /* inclusive of n */
        if (cap == 0)
        {
            cache = (seq_t *)malloc(need * sizeof(seq_t));
        }
        else
        {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        }
        cap = need;
        /* Initialise the first two entries if not already done */
        if (maxc < 1)
        {
            cache[0] = 0;
            if (cap > 1)
                cache[1] = 1;
            maxc = (cap > 1) ? 1 : 0;
        }
    }

    /* If the value is already cached return it directly */
    if (n <= maxc)
        return cache[n];

    /* Compute values that have not yet been computed */
    for (idx_t i = maxc + 1; i <= n; ++i)
    {
        if (i < 2)        /* base cases for safety */
        {
            cache[i] = (i == 0) ? 0 : 1;
            continue;
        }
        seq_t coeff1 = (seq_t)(3 * i - 2);
        seq_t coeff2 = (seq_t)(3 * (i - 1));
        seq_t term1  = coeff1 * cache[i - 1];
        seq_t term2  = coeff2 * cache[i - 2];
        cache[i] = term1 - term2 + (seq_t)1;
    }
    maxc = n;
    return cache[n];
}
