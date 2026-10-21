/*
 * A271484  Expansion of x^5/((1-x^2)*(1-x^4))+x^10/((1-x^4)*(1-x^6)).
 * https://oeis.org/A271484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-6)-a(n-10) for n>10, with a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=0, a(7)=1, a(8)=0, a(9)=2,
 *            a(10)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* cached values */
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of filled entries */

/* Initial terms a(0)..a(10) */
static const seq_t init_vals[11] = {0,0,0,0,0,1,0,1,0,2,1};

seq_t a(idx_t n)
{
    /* direct return for the first 11 terms */
    if ((size_t)n < 11)
        return init_vals[n];

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 16;               /* start with some spare space */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        cache_len = 11;
    }

    /* grow the cache if the requested index is beyond current capacity */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;            /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
