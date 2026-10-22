/*
 * A311593  Coordination sequence Gal.6.192.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, with initial
 *            values a(0) = 1, a(1) = 4, a(2) = 8, a(3) = 12, a(4) = 18, a(5)
 *            = 22, a(6) = 26, a(7) = 30, a(8) = 36, a(9) = 40, a(10) = 44,
 *            a(11) = 48, a(12) = 52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    1, 4, 8, 12, 18, 22, 26, 30, 36, 40, 44, 48, 52
};

static seq_t *cache = NULL;          /* memoised values */
static size_t cache_cap = 0;          /* allocated size of cache */
static idx_t max_computed = 12;       /* highest index computed so far */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n < 13)
        return init[n];

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate enough space for the requested index */
        cache_cap = (size_t)((n + 1) > 13 ? (n + 1) : 13);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 13; ++i)
            cache[i] = init[i];
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute missing values using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
    }
    max_computed = n;

    return cache[n];
}
