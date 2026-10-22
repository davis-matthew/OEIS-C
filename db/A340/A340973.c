/*
 * A340973  Generating function Sum_{n >= 0} a(n)*x^n =
 *          1/sqrt((1-x)*(1-13*x)).
 * https://oeis.org/A340973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 7; for n > 1: n*a(n) = 7*(2*n-1)*a(n-1) -
 *            13*(n-1)*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure base values are stored before any computation. */
    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    }

    /* If the requested index is already computed, return it. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to at least n+1 entries. */
    idx_t new_len = n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

    /* Compute terms from the current length up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[i] = (seq_t)1;
            continue;
        }
        if (i == 1)
        {
            cache[i] = (seq_t)7;
            continue;
        }

        /* Use a larger integer type for the intermediate if seq_t
           has 64 bits or fewer to avoid overflow. */
        if (sizeof(seq_t) <= 8)    /* 64‑bit or less */
        {
            __uint128_t term1 = (__uint128_t)7 * (__uint128_t)(2*i - 1) * (__uint128_t)cache[i-1];
            __uint128_t term2 = (__uint128_t)13 * (__uint128_t)(i - 1) * (__uint128_t)cache[i-2];
            __uint128_t numer  = term1 - term2;
            cache[i] = (seq_t)(numer / (__uint128_t)i);
        }
        else                     /* arbitrary‑precision seq_t */
        {
            seq_t term1 = (seq_t)7 * (seq_t)(2*i - 1);
            term1 *= cache[i-1];
            seq_t term2 = (seq_t)13 * (seq_t)(i - 1);
            term2 *= cache[i-2];
            seq_t numer = term1 - term2;
            cache[i] = numer / (seq_t)i;
        }
    }

    cache_len = new_len;
    return cache[n];
}
