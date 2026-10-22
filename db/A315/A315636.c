/*
 * A315636  Coordination sequence Gal.6.346.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6, a(2) = 12, a(3) = 16, a(4) = 20, a(5) = 26,
 *            a(6) = 30, a(7) = 36, a(8) = 40, a(9) = 44, a(10) = 50, a(11) =
 *            56, a(12) = 62, and for n > 12: a(n) = a(n-1) + a(n-11) -
 *            a(n-12) This implements the OEIS sequence A315636
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    (seq_t)1,  /* a(0) */
    (seq_t)6,  /* a(1) */
    (seq_t)12, /* a(2) */
    (seq_t)16, /* a(3) */
    (seq_t)20, /* a(4) */
    (seq_t)26, /* a(5) */
    (seq_t)30, /* a(6) */
    (seq_t)36, /* a(7) */
    (seq_t)40, /* a(8) */
    (seq_t)44, /* a(9) */
    (seq_t)50, /* a(10) */
    (seq_t)56, /* a(11) */
    (seq_t)62  /* a(12) */
};

static seq_t *cache = NULL;      /* memoised values */
static size_t cache_size = 0;    /* number of values stored so far */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 13)
        return init[(size_t)n];

    /* Ensure cache is at least large enough for the first 13 values */
    if (cache_size < 13) {
        size_t newsize = 13;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)
            abort();              /* out of memory */
        cache = tmp;
        for (size_t i = cache_size; i < 13; ++i)
            cache[i] = init[i];
        cache_size = 13;
    }

    /* Expand cache if needed */
    size_t target = (size_t)n;
    if (target + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (target + 1) * sizeof(seq_t));
        if (!tmp)
            abort();              /* out of memory */
        cache = tmp;
        cache_size = target + 1;
    }

    /* Compute any missing terms */
    for (size_t i = 13; i <= target; ++i) {
        /* a(i) = a(i-1) + a(i-11) - a(i-12) */
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }

    return cache[target];
}
