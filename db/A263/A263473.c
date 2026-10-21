/*
 * A263473  Total number of positive integers < 10^n with multiplicative
 *          digital root value 5.
 * https://oeis.org/A263473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) for n > 10; a(1) = 1, a(2) = 7, ..., a(10) = 243529
 *            (initial values are stored in a static table)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 27, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* static cache for already computed terms; index 0 unused. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_comp = 0;   /* largest n already computed; 0 means none */

    if (n == 0)
        return (seq_t)0;          /* undefined for n<=0, return 0 */

    /* compute the required array size (index n needs n+1 slots including 0) */
    size_t need_cap = (size_t)n + 1;
    if (need_cap < 11) need_cap = 11;   /* we want room for the first 10 values */

    /* allocate or grow the cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc(need_cap * sizeof(seq_t));
        cache_cap = need_cap;
    } else if (cache_cap < need_cap) {
        cache = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
        cache_cap = need_cap;
    }

    /* initialise first 10 values on the first call (or when cache is expanded) */
    if (max_comp == 0) {
        const seq_t init[11] = {
            (seq_t)0,               /* unused 0 index */
            (seq_t)1,               /* a(1)  */
            (seq_t)7,               /* a(2)  */
            (seq_t)40,              /* a(3)  */
            (seq_t)172,             /* a(4)  */
            (seq_t)607,             /* a(5)  */
            (seq_t)2073,            /* a(6)  */
            (seq_t)7414,            /* a(7)  */
            (seq_t)26070,           /* a(8)  */
            (seq_t)84099,           /* a(9)  */
            (seq_t)243529           /* a(10) */
        };
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init[i];
        max_comp = 10;
    }

    /* if the requested term is already known, return it */
    if (n <= max_comp)
        return cache[n];

    /* coefficients of the linear recurrence */
    const int coeff[10] = {
        10,  -45,   120,  -210,   252,
        -210, 120,  -45,    10,   -1
    };

    /* compute terms from max_comp+1 up to n */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        /* only use the recurrence when k > 10 (it holds for all k>10) */
        if (k <= 10) {
            /* values up to 10 were already initialised; skip */
            continue;
        }
        seq_t pos = 0;  /* sum of positive weighted terms */
        seq_t neg = 0;  /* sum of negative weighted terms (absolute value) */
        for (int j = 0; j < 10; ++j) {
            idx_t prev = k - j - 1;  /* (k-1-j) */
            seq_t term = cache[prev];
            int c = coeff[j];
            if (c >= 0)
                pos += (seq_t)c * term;
            else
                neg += (seq_t)(-c) * term;
        }
        cache[k] = pos - neg;
    }

    max_comp = n;
    return cache[n];
}
