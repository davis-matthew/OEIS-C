/*
 * A312549  Coordination sequence Gal.4.61.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=14, a(4)=18, a(5)=24, a(6)=28,
 *            a(7)=32, a(8)=36; for n > 8: a(n)=a(n-1)+a(n-7)-a(n-8)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a static cache holding all previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* base values for n = 0..8 */
    static const seq_t base[9] = {1,4,8,14,18,24,28,32,36};

    /* lazy initialization of the cache */
    if (!cache) {
        cache_cap = 64;                      /* initial capacity */
        cache_len = 9;                       /* we store base[0..8] */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed – defer to caller */
        memcpy(cache, base, 9 * sizeof(seq_t));
    }

    /* grow the array if needed */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed – defer to caller */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values forward */
    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 9) {
                /* already initialized from base */
                continue;
            }
            /* a(i) = a(i-1) + a(i-7) - a(i-8) */
            cache[i] = cache[i-1] + cache[i-7] - cache[i-8];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
