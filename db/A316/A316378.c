/*
 * A316378  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +10*a(n-2) +12*a(n-3) -46*a(n-4) -52*a(n-5)
 *            +14*a(n-6) -57*a(n-7) -58*a(n-8) -38*a(n-9) +33*a(n-10)
 *            +61*a(n-11) -7*a(n-12) +5*a(n-13) -9*a(n-14) +2*a(n-15) (for
 *            n>16) * Initial values: a(1)=1, a(2)=23, a(3)=83, a(4)=464,
 *            a(5)=2280, * a(6)=11423, a(7)=57453, a(8)=288496, a(9)=14478...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* 1‑based cache of all computed terms */
    static size_t cache_cap = 0;         /* current capacity of cache */
    static idx_t max_cached = 0;         /* largest index currently computed */

    /* initial values for n = 1 … 16 */
    static const seq_t init[16] = {
        1ULL,          /* a(1) */
        23ULL,         /* a(2) */
        83ULL,         /* a(3) */
        464ULL,        /* a(4) */
        2280ULL,       /* a(5) */
        11423ULL,      /* a(6) */
        57453ULL,      /* a(7) */
        288496ULL,     /* a(8) */
        1447883ULL,    /* a(9) */
        7274348ULL,    /* a(10) */
        36527122ULL,   /* a(11) */
        183441729ULL,  /* a(12) */
        921291768ULL,  /* a(13) */
        4626743261ULL, /* a(14) */
        23236097303ULL,/* a(15) */
        116694404423ULL/* a(16) */
    };

    /* coefficients of the linear recurrence, indexed 0 … 14 for a(n‑1)…a(n‑15) */
    static const int coeff[15] = {
        3,   10,   12, -46, -52,  14, -57, -58, -38,  33,
       61,   -7,    5,  -9,    2
    };

    /* ensure cache exists and is large enough for the requested index */
    if (cache == NULL)
    {
        /* at least 16 elements are needed for the initial values */
        ensure_cache(16, &cache, &cache_cap);

        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init[i - 1];
        max_cached = 16;
    }
    else if (n > cache_cap)
    {
        /* enlarge cache if necessary */
        ensure_cache(n, &cache, &cache_cap);
    }

    /* return stored value if it is already computed */
    if (n <= max_cached)
        return cache[n];

    /* compute all terms up to n, using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t pos = 0;
        seq_t neg = 0;
        /* terms a(i‑1), a(i‑2), …, a(i‑15) correspond to cache[i‑1], …, cache[i‑15] */
        for (int k = 0; k < 15; ++k)
        {
            seq_t term = cache[i - k - 1];
            int c = coeff[k];
            if (c >= 0)
                pos += (seq_t)c * term;
            else
                neg += (seq_t)(-c) * term;
        }
        cache[i] = pos - neg;
    }
    max_cached = n;
    return cache[n];
}
