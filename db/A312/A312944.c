/*
 * A312944  Coordination sequence Gal.6.245.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 a(0) = 1, a(1) = 4,
 *            a(2) = 9, a(3) = 13, a(4) = 18, a(5) = 23, a(6) = 27, a(7) = 32,
 *            a(8) = 37, a(9) = 41, a(10) = 46, a(11) = 50, a(12) = 54
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 4, 9, 13, 18, 23, 27, 32, 37, 41, 46, 50, 54
};

static seq_t *cache = NULL;
static size_t cache_size = 0;      /* allocated size of cache array */
static size_t computed_len = 0;    /* highest index computed + 1 */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold index n */
    size_t needed = (size_t)n + 1;
    if (needed < 13) needed = 13;          /* at least the initial 13 values */

    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();               /* allocation failed */
        cache_size = needed;
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        computed_len = 13;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = needed;
    }

    /* In case the cache was previously smaller than 13, copy initial values */
    if (computed_len < 13) {
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        computed_len = 13;
    }

    /* Compute missing values up to n using the recurrence */
    if (computed_len <= (size_t)n) {
        for (idx_t i = (idx_t)computed_len; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
        }
        computed_len = (size_t)n + 1;
    }

    return cache[n];
}
