/*
 * A274006  Largest proper prime power divisor of n, or 1 if n is squarefree.
 * https://oeis.org/A274006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Largest proper prime power divisor of n, or 1 if n is squarefree
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/build_spf.h"

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static cache for computed values and for SPF array. */
    static seq_t *cache   = NULL;   /* cache[1]..cache[max] */
    static size_t cache_cap = 0;    /* allocated size (including dummy 0 index) */
    static idx_t  *spf     = NULL;   /* smallest prime factor array (0..max) */
    static idx_t  spf_cap  = 0;      /* allocated size of spf array */
    static idx_t   current_max = 0;  /* highest n already filled in cache */

    if (n < 1) return 1; /* safety, should not happen */

    /* Expand SPF array if necessary. */
    if (n >= spf_cap)
    {
        /* build_spf builds array of size limit+1 with indices 0..limit. */
        build_spf(n, &spf);
        spf_cap = n + 1;
    }

    /* Ensure cache has room and compute missing entries. */
    if (n >= cache_cap)
    {
        ensure_cache(n + 1, &cache, &cache_cap);
    }

    if (n <= current_max)
        return cache[n]; /* already computed */

    /* Compute values from current_max+1 up to n. */
    for (idx_t x = current_max + 1; x <= n; ++x)
    {
        seq_t max_pow = 1;
        idx_t temp = x;
        while (temp > 1)
        {
            idx_t p  = spf[temp];
            idx_t cnt = 0;
            seq_t pow = 1;
            while (temp % p == 0)
            {
                temp /= p;
                ++cnt;
                pow *= p; /* accumulate p^cnt */
            }
            if (cnt >= 2 && pow > max_pow)
                max_pow = pow;
        }
        cache[x] = max_pow;
    }

    current_max = n;
    return cache[n];
}
