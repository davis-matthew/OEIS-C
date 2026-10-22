/*
 * A313962  Coordination sequence Gal.3.53.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (recurrence):
        a(0) = 1, a(1) = 5, a(2) = 10, a(3) = 17, a(4) = 21, a(5) = 27,
        a(6) = 31, a(7) = 38, a(8) = 43,
        a(n) = 2*a(n-1) - a(n-2) - a(n-3) + 2*a(n-4) - a(n-5) - a(n-6)
               + 2*a(n-7) - a(n-8)   for n > 8
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* capacity of cache array */

    /* Initial values for n = 0..8 */
    static const seq_t init[9] = {
        1, 5, 10, 17, 21, 27, 31, 38, 43
    };

    /* Grow cache if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if they are not already cached */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 9) {
            cache[i] = init[i];
        } else {
            seq_t s =   2 * cache[i - 1]
                    -   cache[i - 2]
                    -   cache[i - 3]
                    + 2 * cache[i - 4]
                    -   cache[i - 5]
                    -   cache[i - 6]
                    + 2 * cache[i - 7]
                    -   cache[i - 8];
            cache[i] = s;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
