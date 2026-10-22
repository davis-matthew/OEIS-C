/*
 * A310027  Coordination sequence Gal.5.41.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-19) - a(n-20) + a(n-21) -
 *            a(n-22) for n > 22 Initial values: a(0)=1, a(1)=3, a(2)=5,
 *            a(3)=8, a(4)=11, a(5)=14, a(6)=18, a(7)=21, a(8)=23, a(9)=25,
 *            a(10)=28, a(11)=32, a(12)=34, a(13)=35, a(14)=39, a(15)=44,
 *            a(16)=47, a(17)=48, a(18)=50, a(19)=55, a(20)=58, a(...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[24] = {
        1, 3, 5, 8, 11, 14, 18, 21, 23, 25,
        28, 32, 34, 35, 39, 44, 47, 48, 50, 55,
        58, 58, 61, 66
    };
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;
    static idx_t cache_computed = 23;  /* last index for which a value is known */

    /* First allocation: at least 64 slots, copy initial seeds */
    if (!cache) {
        cache_alloc = 64;
        if (cache_alloc < 24) cache_alloc = 24;
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
        /* copy initial values */
        for (idx_t i = 0; i < 24; ++i) {
            cache[i] = init[i];
        }
    }
    /* Grow the buffer if needed */
    else if ((size_t)n >= cache_alloc) {
        size_t new_alloc = (n + 1 > 64) ? (n + 1) : 64;
        if (new_alloc < cache_alloc * 2) new_alloc = cache_alloc * 2;
        cache = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        cache_alloc = new_alloc;
    }

    /* Compute any missing values by recurrence */
    if (n > cache_computed) {
        for (idx_t i = cache_computed + 1; i <= n; ++i) {
            /* recurrence uses only indices that are already defined for i>23 */
            cache[i] = cache[i-1] - cache[i-2] + cache[i-3]
                     + cache[i-19] - cache[i-20] + cache[i-21] - cache[i-22];
        }
        cache_computed = n;
    }

    return cache[n];
}
