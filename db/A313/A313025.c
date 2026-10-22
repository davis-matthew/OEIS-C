/*
 * A313025  Coordination sequence Gal.5.65.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) (n > 10)
 *            Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=18,
 *            a(5)=22, a(6)=26, a(7)=30, a(8)=35, a(9)=40, a(10)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Base values for indices 0..10 */
    const seq_t base[11] = {
        1ULL, 4ULL, 9ULL, 14ULL, 18ULL,
        22ULL, 26ULL, 30ULL, 35ULL, 40ULL, 44ULL
    };

    /* Fast path if already computed */
    if (n < cache_len)
        return cache[n];

    /* Allocate cache initially */
    if (!cache) {
        cache_cap = 64;                      /* arbitrary start size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 10) {
            cache[i] = base[i];
        } else {
            cache[i] =
                (2 * cache[i - 1] - 2 * cache[i - 2] + 2 * cache[i - 3] -
                 2 * cache[i - 4] + 2 * cache[i - 5] - 2 * cache[i - 6] +
                 2 * cache[i - 7] - 2 * cache[i - 8] + 2 * cache[i - 9] -
                 cache[i - 10]);
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
