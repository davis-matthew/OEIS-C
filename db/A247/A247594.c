/*
 * A247594  a(n) = a(n-1) + a(n-2) + 3*a(n-3) with a(0) = 1, a(1) = 2, a(2) =
 *          5.
 * https://oeis.org/A247594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(n)=a(n-1)+a(n-2)+3*a(n-3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values as seq_t */
    const seq_t v0 = 1, v1 = 2, v2 = 5;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cached = 0;

    /* Initialise cache with the first three terms */
    if (!cache) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = v0;
        cache[1] = v1;
        cache[2] = v2;
        max_cached = 2;
    }

    /* Grow cache if necessary and compute missing terms */
    if (n > max_cached) {
        if ((size_t)(n + 1) > cap) {
            size_t new_cap = cap;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (size_t i = 0; i <= (size_t)max_cached; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cap = new_cap;
        }
        for (idx_t i = max_cached + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3] * 3;
        max_cached = n;
    }

    return cache[n];
}
