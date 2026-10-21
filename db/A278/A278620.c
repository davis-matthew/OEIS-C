/*
 * A278620  Expansion of x/(1 - 99*x + 99*x^2 - x^3).
 * https://oeis.org/A278620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=99, a(n)=99*a(n-1)-99*a(n-2)+a(n-3) for n>2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static idx_t    cache_len = 0;       /* number of values stored */
    static idx_t    cache_cap = 0;       /* capacity of cache */
    static seq_t *  cache = NULL;        /* array of cached terms */

    /* allocate cache on first call */
    if (!cache)
    {
        cache_cap = 4;
        cache_len = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* compute up to n if necessary */
    while (cache_len <= n)
    {
        /* ensure space */
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        seq_t val;
        if (cache_len == 0)        val = (seq_t)0;
        else if (cache_len == 1)   val = (seq_t)1;
        else if (cache_len == 2)   val = (seq_t)99;
        else
        {
            /* recurrence: a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) */
            val = (seq_t)99 * cache[cache_len-1]
                 - (seq_t)99 * cache[cache_len-2]
                 + cache[cache_len-3];
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
