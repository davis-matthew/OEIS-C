/*
 * A296660  Expansion of the e.g.f. exp(-2*x)/(1-4*x).
 * https://oeis.org/A296660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A296660 – Expansion of the e.g.f. exp(-2*x)/(1-4*x)
 *
 *  Recurrence:  a(0) = 1
 *               a(n+1) = 4*(n+1)*a(n) + (-2)^(n+1)
 *  The code uses this recurrence and memoises all previously computed
 *  values, so any call order is supported.  A dynamic array is
 *  allocated so that the implementation works with arbitrary-precision
 *  seq_t types as well as fixed-width unsigned integers.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* memoisation cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of valid terms in cache [0 .. cache_len-1] */
static size_t cache_cap = 0;   /* allocated capacity for cache array        */
static seq_t pow2 = 1;         /* 2^i for the largest i that has been
                                 processed.  Initially 2^0 = 1.           */

/* ensure that the cache contains at least (n+1) elements */
static void ensure_cache(idx_t n)
{
    /* grow capacity if needed – double until sufficient */
    if (cache_len > n)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;          /* casting to seq_t* is required for C++17 */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache(n);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
        pow2 = 1;              /* 2^0 */
    }

    /* already known */
    if ((size_t)n < cache_len)
        return cache[n];

    ensure_cache(n);

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* advance pow2 and compute sign for (-2)^i */
        pow2 <<= 1;                       /* pow2 = 2^i */
        int sign = (i & 1) ? -1 : 1;      /* (-1)^i */

        seq_t term = cache[i-1] *
                     ((seq_t)(4 * i));     /* 4*i*a(i-1) */

        if (sign == 1)
            term += pow2;                 /* add 2^i */
        else
            term -= pow2;                 /* subtract 2^i */

        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
