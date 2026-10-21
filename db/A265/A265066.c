/*
 * A265066  Coordination sequence for (2,5,7) tiling of hyperbolic plane.
 * https://oeis.org/A265066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 53, u64 n <= 108,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265066: Coordination sequence for the (2,5,7) tiling of the hyperbolic plane.
   The generating function is
   (x^6+x^5+x^4+x^3+x^2+x+1)*(x^4+x^3+x^2+x+1)*(x+1)^2
   ------------------------------------------------------------
   ------------------------------------------------------------
   For n ≥ 13 the sequence satisfies the linear recurrence
     a(n) = a(n−3) + 2 a(n−4) + 3 a(n−5) + 3 a(n−6) + 3 a(n−7)
            + 2 a(n−8) + a(n−9) − a(n−1) − a(n−11) − a(n−12)
   The first 13 terms are given explicitly and provide the
   initial conditions.  No further symbols from the utilities are used.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const seq_t init[13] = {
    (seq_t)1,   /* a(0) */
    (seq_t)3,   /* a(1) */
    (seq_t)5,   /* a(2) */
    (seq_t)8,   /* a(3) */
    (seq_t)13,  /* a(4) */
    (seq_t)20,  /* a(5) */
    (seq_t)30,  /* a(6) */
    (seq_t)45,  /* a(7) */
    (seq_t)67,  /* a(8) */
    (seq_t)100, /* a(9) */
    (seq_t)149, /* a(10) */
    (seq_t)221, /* a(11) */
    (seq_t)329  /* a(12) */
};

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Return one of the first 13 terms directly when possible. */
    if (n < 13)
        return init[(idx_t)n];

    /* Lazy initialization of the cache.  The cache never shrinks. */
    if (cache == NULL)
    {
        cache_cap = n + 1;
        if (cache_cap < 16U)
            cache_cap = 16U;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* out of memory – unlikely in the test harness */
        memcpy(cache, init, 13 * sizeof(seq_t));
        cache_len = 13;
    }

    /* Grow the cache if the requested index is beyond its current size. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;
        /* Zero the newly allocated part */
        memset(tmp + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        /* sum of positive contributions */
        seq_t pos = cache[i-3];
        pos += cache[i-4]; pos += cache[i-4];
        pos += cache[i-5]; pos += cache[i-5]; pos += cache[i-5];
        pos += cache[i-6]; pos += cache[i-6]; pos += cache[i-6];
        pos += cache[i-7]; pos += cache[i-7]; pos += cache[i-7];
        pos += cache[i-8]; pos += cache[i-8];
        pos += cache[i-9];

        /* sum of negative contributions */
        seq_t sub = cache[i-1];
        sub += cache[i-11];
        sub += cache[i-12];

        cache[i] = pos - sub;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
