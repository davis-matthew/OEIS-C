/*
 * A317890  Number of nX2 0..1 arrays with every element unequal to 2, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) + 3*a(n-4) + a(n-5) for n >
 *            6 Initial values: a(1) = 0 a(2) = 1 a(3) = 1 a(4) = 2 a(5) = 5
 *            a(6) = 16 (Values for n ≥ 1 are cached and computed on demand.)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    /* the highest index that has been initialized in the cache */
    static idx_t cache_last = 0;

    if (n == 0) /* sequence starts at index 1; return 0 for safety */
        return 0;

    /* enlarge cache if necessary */
    if (cache == NULL || cache_last < n) {
        idx_t new_size = (n >= 6) ? (n + 1) : (6 + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* out of memory: return 0 as a fallback */
        cache = new_cache;

        /* initialize base values up to index 6 if they haven't been set yet */
        if (cache_last < 1) {
            for (idx_t i = 1; i <= 1 && i <= n; ++i) cache[i] = (seq_t)0;
        }
        if (cache_last < 2) {
            for (idx_t i = 2; i <= 2 && i <= n; ++i) cache[i] = (seq_t)1;
        }
        if (cache_last < 3) {
            for (idx_t i = 3; i <= 3 && i <= n; ++i) cache[i] = (seq_t)1;
        }
        if (cache_last < 4) {
            for (idx_t i = 4; i <= 4 && i <= n; ++i) cache[i] = (seq_t)2;
        }
        if (cache_last < 5) {
            for (idx_t i = 5; i <= 5 && i <= n; ++i) cache[i] = (seq_t)5;
        }
        if (cache_last < 6) {
            for (idx_t i = 6; i <= 6 && i <= n; ++i) cache[i] = (seq_t)16;
        }

        /* compute further terms up to n using the recurrence */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            if (i <= 6) {
                /* base values already set above */
                continue;
            }
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            seq_t v5 = cache[i - 5];
            /* a(i) = 2*a(i-1) + a(i-2) + 2*a(i-3) + 3*a(i-4) + a(i-5) */
            seq_t result = (seq_t)2 * v1 + v2 + (seq_t)2 * v3 + (seq_t)3 * v4 + v5;
            cache[i] = result;
        }
        cache_last = n;
    }

    return cache[n];
}
