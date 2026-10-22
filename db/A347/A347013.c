/*
 * A347013  E.g.f.: exp(x) / (1 - 5 * x)^(1/5).
 * https://oeis.org/A347013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n+1) = (5*n + 2)*a(n) - 5*n*a(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence grows rapidly; we compute terms on demand
   and cache them in a static array. The recurrence is
   derived from the differential equation satisfied by the
   exponential generating function G(x)=exp(x)/(1-5x)^(1/5). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  cache_size = 0;  /* number of terms stored (>=0) */

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Ensure cache is large enough to hold up to n */
    if (cache_size <= n)
    {
        /* Expand capacity: double until big enough */
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < needed)
            new_cap *= 2;

        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;   /* allocation failure → 0 */

        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* free old cache */
        free(cache);

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If cache was just created, initialize a(0)=1 and a(1)=2 */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* Recurrence: a(i) = (5*(i-1)+2)*a(i-1) - 5*(i-1)*a(i-2) */
        idx_t k = i - 1;                      /* k = n in the formula */
        seq_t factor1 = (seq_t)(5 * k + 2);    /* (5n+2) */
        seq_t factor2 = (seq_t)(5 * k);        /* 5n   */

        /* term1 = factor1 * a(k) */
        seq_t term1 = cache[i - 1] * factor1;

        /* term2 = factor2 * a(k-1) */
        seq_t term2 = cache[i - 2] * factor2;

        cache[i] = term1 - term2;
    }

    cache_size = n + 1;   /* we now have values up to n */
    return cache[n];
}
