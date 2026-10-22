/*
 * A312943  Coordination sequence Gal.4.41.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) for all n
 *            > 6, with initial values a(0)=1, a(1)=4, a(2)=9, a(3)=13,
 *            a(4)=18, a(5)=23, a(6)=27
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* base values */
    static const seq_t base[7] = {1, 4, 9, 13, 18, 23, 27};

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;             /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure – unlikely */
        for (idx_t i = 0; i < 7; ++i) cache[i] = base[i];
        cache_size = 7;
    }

    /* if we already have the value, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (idx_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = base[i];
        } else {
            cache[i] = 2 * cache[i - 1]
                       - cache[i - 2]
                       - cache[i - 4]
                       + 2 * cache[i - 5]
                       - cache[i - 6];
        }
    }
    cache_size = n + 1;

    return cache[n];
}
