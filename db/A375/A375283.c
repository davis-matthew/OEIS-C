/*
 * A375283  Expansion of 1/((1 - x - x^4)^2 - 4*x^5).
 * https://oeis.org/A375283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) + 2*a(n-5) - a(n-8), a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=16, a(6)=35, a(7)=68
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n < 8 */
    static const seq_t init[8] = {1, 2, 3, 4, 7, 16, 35, 68};

    if (n < 8)
        return init[n];

    /* Static cache that grows as needed */
    static seq_t *cache   = NULL;
    static idx_t  size    = 0;
    idx_t old_size;

    /* First allocation with the initial eight values */
    if (size == 0) {
        size = 8;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* Return already computed value if available */
    if (n < size)
        return cache[n];

    /* Grow the cache to hold all computed values up to n */
    old_size = size;
    size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
    if (!new_cache)
        return 0;              /* allocation failure */
    cache = new_cache;

    /* Compute new terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1]
                 -  cache[i - 2]
                 + 2 * cache[i - 4]
                 + 2 * cache[i - 5]
                 -  cache[i - 8];
    }

    return cache[n];
}
