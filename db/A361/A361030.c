/*
 * A361030  a(n) = 20160*(3*n)!/(n!*(n+3)!^2).
 * https://oeis.org/A361030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n + 3)^2 * a(n) = 3 * (3*n - 1) * (3*n - 2) * a(n-1), with a(0)
 *            = 560. The sequence values are always integral
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Simple memoisation: cache already computed terms. */
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored */

    /* Extend the cache if necessary. */
    if (n >= (idx_t)cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;         /* allocation failed: sentinel 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise first term if not yet done. */
        if (cache_len == 0) {
            cache[0] = (seq_t)560;
            cache_len = 1;
        }

        /* Compute remaining terms up to n using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Recurrence uses i as the current n. */
            /*  num = 3 * (3*i - 1) * (3*i - 2)                 */
            /*  den = (i + 3) * (i + 3)                        */
            /*  a(i) = a(i-1) * num / den                     */

            /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds. */
            __int128 num = 3;
            num *= (3 * (idx_t)i - 1);
            num *= (3 * (idx_t)i - 2);

            idx_t den_term = i + 3;
            __int128 den = (__int128)den_term * den_term;

            __int128 prod = (__int128)cache[i - 1] * num;
            cache[i] = (seq_t)(prod / den);
        }

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
