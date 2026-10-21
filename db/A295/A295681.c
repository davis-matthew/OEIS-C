/*
 * A295681  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 0, a(1) = 1, a(2) =
 *          0, a(3) = 2.
 * https://oeis.org/A295681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=0, a(1)=1, a(2)=0, a(3)=2
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) return 0;  /* memory allocation failed; return 0 as a safe fallback */

        /* initialize new slots to zero */
        for (idx_t i = old_size; i < cache_size; ++i)
            cache[i] = 0;

        /* initialize base cases */
        if (cache_size > 0) cache[0] = 0;
        if (cache_size > 1) cache[1] = 1;
        if (cache_size > 2) cache[2] = 0;
        if (cache_size > 3) cache[3] = 2;

        /* compute missing terms up to n */
        for (idx_t i = 4; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 4];
    }

    return cache[n];
}
