/*
 * A314234  Coordination sequence Gal.6.332.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) (for n > 12) Initial values:
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=17, a(4)=23, a(5)=28, a(6)=32,
 *            a(7)=37, a(8)=43, a(9)=49, a(10)=55, a(11)=60, a(12)=65
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 5, 11, 17, 23, 28, 32, 37, 43, 49, 55, 60, 65
};

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    /* Return known initial values directly. */
    if (n < 13)
        return init_vals[n];

    /* Allocate and initialize cache on first call. */
    if (cache_len == 0) {
        cache_len = 13;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            exit(1);
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    /* Extend cache to index n if needed. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i >= 13 for the recurrence */
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
        cache_len = new_len;
    }

    return cache[n];
}
