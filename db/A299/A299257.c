/*
 * A299257  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 3.12.12 2D tiling (cf. A250122).
 * https://oeis.org/A299257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 7*a(n-3) - 7*a(n-4) + 5*a(n-5) -
 *            3*a(n-6) + a(n-7); with initial values: a(0)=1, a(1)=5, a(2)=12,
 *            a(3)=22, a(4)=36, a(5)=56, a(6)=82, a(7)=111, a(8)=144, a(9)=183
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static const seq_t init[10] = {1,5,12,22,36,56,82,111,144,183};
    size_t i;

    /* return directly if n is one of the known initial terms */
    if (n < 10)
        return init[n];

    /* first call: allocate a cache and fill the first 10 terms */
    if (!cache) {
        cache_cap = (size_t)(n + 1) * 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure – caller will treat as 0 */
        for (i = 0; i < 10; ++i)
            cache[i] = init[i];
        for (i = 10; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        for (i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (i = 10; i <= (size_t)n; ++i) {
        if (cache[i] == 0) {
            seq_t pos = 3 * cache[i - 1] + 7 * cache[i - 3] +
                        5 * cache[i - 5] + cache[i - 7];
            seq_t neg = 5 * cache[i - 2] + 7 * cache[i - 4] +
                        3 * cache[i - 6];
            cache[i] = pos - neg;
        }
    }
    return cache[n];
}
