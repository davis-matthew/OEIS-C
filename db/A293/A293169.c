/*
 * A293169  a(n) = Sum_{k=0..n} binomial(k, 6*(n-k)).
 * https://oeis.org/A293169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1 a(n) =
 *            6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
 *            + a(n-7) for n ≥ 7 (OEIS A293169)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 72, u128 n <= 141,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;  /* pointer to allocated array */
    static size_t cache_cap = 0;    /* current capacity of the array */
    static size_t computed  = 0;    /* number of values already computed */

    /* Ensure space for up to index n */
    if (n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* Compute missing values up to n if required */
    for (size_t i = computed; i <= (size_t)n; ++i)
    {
        if (i <= 6)
        {
            cache[i] = (seq_t)1;
        }
        else
        {
            /* Use special handling for 64‑bit seq_t to avoid overflow.
               For other seq_t sizes (arbitrary precision) we rely on the
               built‑in arithmetic of seq_t. */
            if (sizeof(seq_t) == 8)
            {
                /* 128‑bit intermediate to preserve exactness */
                __int128 t1 = (__int128)6 * (__int128)cache[i-1];
                __int128 t2 = (__int128)15 * (__int128)cache[i-2];
                __int128 t3 = (__int128)20 * (__int128)cache[i-3];
                __int128 t4 = (__int128)15 * (__int128)cache[i-4];
                __int128 t5 = (__int128)6  * (__int128)cache[i-5];
                __int128 t6 = (__int128)cache[i-6];
                __int128 t7 = (__int128)cache[i-7];
                __int128 res = t1 - t2 + t3 - t4 + t5 - t6 + t7;
                cache[i] = (seq_t)res;
            }
            else
            {
                seq_t term1 = cache[i-1] * (seq_t)6;
                seq_t term2 = cache[i-2] * (seq_t)15;
                seq_t term3 = cache[i-3] * (seq_t)20;
                seq_t term4 = cache[i-4] * (seq_t)15;
                seq_t term5 = cache[i-5] * (seq_t)6;
                seq_t term6 = cache[i-6];
                seq_t term7 = cache[i-7];

                seq_t tmp = term1;
                tmp -= term2;
                tmp += term3;
                tmp -= term4;
                tmp += term5;
                tmp -= term6;
                tmp += term7;

                cache[i] = tmp;
            }
        }
    }
    computed = (size_t)n + 1;
    return cache[n];
}
