/*
 * A348875  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x * A(2*x)).
 * https://oeis.org/A348875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=a(n-1)+∑_{k=0}^{n-1} 2^k · a(k) · a(n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoized computation of A348875 */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;      /* array capacity */
    static idx_t  max_comp   = 0;     /* largest computed index */

    /* Ensure cache for index 0 exists */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        max_comp = 0;
    }

    /* Return a(0) immediately */
    if (n == (idx_t)0)
        return cache[0];

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute all values up to requested n */
    if (n > (idx_t)max_comp)
    {
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t sum  = (seq_t)0;
            seq_t pow2 = (seq_t)1;          /* 2^0 */
            for (idx_t k = 0; k < i; ++k)
            {
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod = prod * pow2;
                sum += prod;
                pow2 <<= 1;                  /* multiply by 2 */
            }
            cache[i] = cache[i - 1] + sum;
        }
        max_comp = n;
    }

    return cache[n];
}
