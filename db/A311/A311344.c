/*
 * A311344  Coordination sequence Gal.5.95.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10, a(0) = 1, a(1) = 4,
 *            a(2) = 8, a(3) = 12, a(4) = 16, a(5) = 22, a(6) = 26, a(7) = 30,
 *            a(8) = 34, a(9) = 38, a(10) = 42
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements already computed */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Convert idx_t to size_t for array indexing */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        /* Increase capacity: either double or set to idx+1 */
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < idx + 1)
            new_cap = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)   /* allocation failed; abort gracefully */
            return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to idx */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0)        cache[i] = 1;
            else if (i == 1)    cache[i] = 4;
            else if (i == 2)    cache[i] = 8;
            else if (i == 3)    cache[i] = 12;
            else if (i == 4)    cache[i] = 16;
            else if (i == 5)    cache[i] = 22;
            else if (i == 6)    cache[i] = 26;
            else if (i == 7)    cache[i] = 30;
            else if (i == 8)    cache[i] = 34;
            else if (i == 9)    cache[i] = 38;
            else if (i == 10)   cache[i] = 42;
            else                cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
