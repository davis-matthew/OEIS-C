/*
 * A310501  Coordination sequence Gal.6.328.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, with a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=16, a(4)=20, a(5)=26, a(6)=30, a(7)=36,
 *            a(8)=40, a(9)=46, a(10)=52, a(11)=56, a(12)=60
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_size = 0;        /* allocated cache size in elements */

static const seq_t init_vals[13] = {
    1, 4, 10, 16, 20, 26, 30, 36, 40, 46, 52, 56, 60
};

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Return first 13 terms via constant lookup to avoid
       constructing cache for tiny indices. */
    if (idx < 13) return init_vals[idx];

    /* Allocate or grow the cache so that it contains [0 .. idx]. */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as a fallback */
            return 0;
        }
        cache = tmp;

        /* Copy initial values into the cache if they are not already there. */
        if (cache_size < 13) {
            size_t start = cache_size;
            if (start > new_size) start = new_size;
            for (size_t i = start; i < (size_t)13 && i < new_size; ++i) {
                cache[i] = init_vals[i];
            }
        }

        /* Compute any new terms beyond the old cache size. */
        size_t start = cache_size > 13 ? cache_size : 13;
        for (size_t i = start; i < new_size; ++i) {
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }

        cache_size = new_size;
    }

    return cache[idx];
}
