/*
 * A396241  Expansion of 1 / (1 - B(B(B(x)))), where B(x) = x/(1-x)^2.
 * https://oeis.org/A396241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=7, a(3)=46, a(4)=295, a(5)=1868,
   a(6)=11746, a(7)=73566, a(8)=459701
   a(n) = 15*a(n-1) - 83*a(n-2) + 220*a(n-3) - 303*a(n-4)
          + 220*a(n-5) - 83*a(n-6) + 15*a(n-7) - a(n-8)   (n>8) */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = (idx_t)-1;      /* index after last computed value */
    static size_t cap = 0;

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* If we already have this value, return it */
    if (max_cached <= n && n < max_cached)
        return cache[n];

    /* Compute missing values from (max_cached) up to n */
    for (idx_t i = (max_cached == (idx_t)-1) ? 0 : max_cached; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)7;
        else if (i == 3)
            cache[i] = (seq_t)46;
        else if (i == 4)
            cache[i] = (seq_t)295;
        else if (i == 5)
            cache[i] = (seq_t)1868;
        else if (i == 6)
            cache[i] = (seq_t)11746;
        else if (i == 7)
            cache[i] = (seq_t)73566;
        else if (i == 8)
            cache[i] = (seq_t)459701;
        else
            cache[i] =
                (seq_t)15  * cache[i-1]
              - (seq_t)83  * cache[i-2]
              + (seq_t)220 * cache[i-3]
              - (seq_t)303 * cache[i-4]
              + (seq_t)220 * cache[i-5]
              - (seq_t)83  * cache[i-6]
              + (seq_t)15  * cache[i-7]
              -          cache[i-8];
    }

    max_cached = n + 1;   /* next index to compute will be n+1 */

    return cache[n];
}
