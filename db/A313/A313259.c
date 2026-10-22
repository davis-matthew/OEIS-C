/*
 * A313259  Coordination sequence Gal.6.204.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 with a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=15, a(4)=20, a(5)=25, a(6)=31, a(7)=36,
 *            a(8)=41, a(9)=47, a(10)=52, a(11)=56, a(12)=60
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A313259 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static const seq_t base[13] = {
    1, 4, 9, 15, 20, 25, 31, 36, 41, 47, 52, 56, 60
};

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i <= 12; ++i)
            cache[i] = base[i];
    }

    if ((size_t)n < cache_cap)
        return cache[n];

    size_t old_cap = cache_cap;
    if (cache_cap <= (size_t)n)
        cache_cap = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));

    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        if (i <= 12)
            continue;
        cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
    }
    return cache[n];
}
