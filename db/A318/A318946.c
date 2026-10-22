/*
 * A318946  Column 1 of triangle A318945.
 * https://oeis.org/A318946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 16*a(n-3) - 4*a(n-4) a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=19
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = (idx_t)-1;   /* indicates uninitialised */

    /* initialise the cache with the first six values */
    if (max_computed == (idx_t)-1) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – fallback to 0 */

        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 5;
        cache[5] = 19;
        max_computed = 5;
    }

    /* for trivial indices return immediately */
    if (n <= 5)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    if (n > max_computed) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed – fallback to 0 */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = 7 * cache[i-1]
                     - 17 * cache[i-2]
                     + 16 * cache[i-3]
                     - 4 * cache[i-4];
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
