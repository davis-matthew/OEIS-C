/*
 * A314838  Coordination sequence Gal.4.52.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8 Initial values:
 *            a(0)=1, a(1)=5, a(2)=9, a(3)=14, a(4)=18, a(5)=23, a(6)=27,
 *            a(7)=32, a(8)=37
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t base[9] = {1,5,9,14,18,23,27,32,37};

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(9 * sizeof(seq_t));
        if (!cache) return 0;                /* out‑of‑memory, should not happen */
        for (idx_t i = 0; i < 9; ++i) cache[i] = base[i];
        cache_size = 9;
    }

    /* grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* out‑of‑memory */
        cache = tmp;

        /* compute missing terms in order */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 9) continue;           /* already filled with base values */
            cache[i] = cache[i-1] + cache[i-7] - cache[i-8];
        }
        cache_size = new_size;
    }

    return cache[n];
}
