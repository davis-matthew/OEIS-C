/*
 * A392578  Expansion of 1 / ((1-x)^3 - x)^2.
 * https://oeis.org/A392578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 26*a(n-3) - 17*a(n-4) + 6*a(n-5) -
 *            a(n-6) Initial values: a(0)=1, a(1)=8, a(2)=42, a(3)=186,
 *            a(4)=755, a(5)=2910
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n < 6 */
    static const seq_t base[6] = {1, 8, 42, 186, 755, 2910};

    if (n < 6)
        return base[n];

    /* memoisation for n >= 6 */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Initialise cache if this is the first use for n >= 6 */
    if (cache_size == 0) {
        /* Reserve at least 6 slots */
        cache_cap = 64;
        while (cache_cap <= n)
            cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        /* store base values */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
        cache_size = 6;
    }

    /* Grow cache capacity if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 8 * cache[i-1]
                 - 22 * cache[i-2]
                 + 26 * cache[i-3]
                 - 17 * cache[i-4]
                 + 6  * cache[i-5]
                 -  cache[i-6];
    }
    cache_size = n + 1;

    return cache[n];
}
