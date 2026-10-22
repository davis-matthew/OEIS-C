/*
 * A315498  Coordination sequence Gal.6.334.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0)=1, a(1)=6,
 *            a(2)=11, a(3)=16, a(4)=22, a(5)=28, a(6)=34, a(7)=40, a(8)=46,
 *            a(9)=51, a(10)=56, a(11)=62, a(12)=68
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 13 terms of the sequence */
    static const seq_t init[13] = {
        1, 6, 11, 16, 22, 28, 34, 40, 46, 51, 56, 62, 68
    };

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_len = 0;   /* number of terms currently stored */

    /* Lazy initialization of cache with the 13 known terms */
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 13; ++i) {
            cache[i] = init[i];
        }
        cache_len = 13;
    }

    /* Ensure cache has capacity to hold terms up to n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute any missing terms using the recurrence */
    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = a(i-1) + a(i-11) - a(i-12) */
            cache[(size_t)i] = cache[(size_t)(i - 1)]
                             + cache[(size_t)(i - 11)]
                             - cache[(size_t)(i - 12)];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
