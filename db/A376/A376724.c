/*
 * A376724  Expansion of 1/((1 - x^3 - x^4)^2 - 4*x^7).
 * https://oeis.org/A376724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) + 2*a(n-4) - a(n-6) + 2*a(n-7) - a(n-8), initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=2, a(5)=0, a(6)=3,
 *            a(7)=10
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* base values */
    static const seq_t base[8] = {1ULL, 0ULL, 0ULL, 2ULL, 2ULL, 0ULL, 3ULL, 10ULL};

    /* initialise cache with base values if not yet done */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return base[n]; /* malloc failure; unlikely */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base[i];
    }

    /* return known base values */
    if (n < 8)
        return cache[n];

    /* ensure cache large enough */
    size_t required_len = (size_t)n + 1;
    if (required_len > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, required_len * sizeof(seq_t));
        if (!tmp) return 0;          /* realloc failure; fallback */
        cache = tmp;
        size_t old_len = cache_len;
        cache_len = required_len;
        /* compute missing terms */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            seq_t term = 0;
            term += 2 * cache[i-3];
            term += 2 * cache[i-4];
            term -= cache[i-6];
            term += 2 * cache[i-7];
            term -= cache[i-8];
            cache[i] = term;
        }
    } else {
        /* compute missing terms if cache exists but not yet populated beyond old length */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t term = 0;
            term += 2 * cache[i-3];
            term += 2 * cache[i-4];
            term -= cache[i-6];
            term += 2 * cache[i-7];
            term -= cache[i-8];
            cache[i] = term;
        }
    }

    return cache[n];
}
