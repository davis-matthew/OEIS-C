/*
 * A313855  Coordination sequence Gal.5.87.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) Initial
 *            terms (n = 0..10): 1, 5, 10, 16, 21, 25, 29, 34, 40, 45, 50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;        /* length of allocated array      */
    static idx_t last_computed = 10;    /* largest n that has been computed */
    static const seq_t base[11] = {1,5,10,16,21,25,29,34,40,45,50};

    /* return known initial terms directly */
    if (n < 11)
        return base[n];

    /* if we have already computed this term, return it */
    if (n <= last_computed)
        return cache[n];

    /* initialise the cache on first use */
    if (!cache) {
        cache_len = 11;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        assert(cache);
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = base[i];
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        assert(tmp);
        cache = tmp;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* odd-offset terms with coefficient +2 */
        seq_t sum_odd = cache[k-1] + cache[k-3] + cache[k-5] +
                         cache[k-7] + cache[k-9];
        sum_odd <<= 1;               /* multiply by 2 */
        sum_odd -= cache[k-10];      /* subtract final -a(k-10) term */

        /* even-offset terms with coefficient -2 */
        seq_t sum_even = cache[k-2] + cache[k-4] +
                          cache[k-6] + cache[k-8];
        sum_even <<= 1;              /* multiply by 2 */

        /* recurrence */
        cache[k] = sum_odd - sum_even;
    }

    last_computed = n;
    return cache[n];
}
