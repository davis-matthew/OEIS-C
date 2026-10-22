/*
 * A314093  Coordination sequence Gal.5.137.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6); a(0) = 1, a(1) = 5, a(2) = 11, a(3) = 16, a(4) = 20,
 *            a(5) = 25, a(6) = 30
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;          /* array of computed a(k) values */
    static size_t cache_len = 0;         /* number of values currently cached (== index+1) */

    /* if already computed, return immediately */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* compute up to index n */
    size_t new_len = (size_t)n + 1;

    /* grow cache */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) /* realloc failed */
        abort();  /* safe abort in both C and C++ */
    cache = tmp;

    /* initialise missing initial terms if needed */
    static const seq_t init[7] = {1,5,11,16,20,25,30};
    if (cache_len < 7) {
        size_t start = cache_len;
        size_t end = new_len < 7 ? new_len : 7;
        for (size_t i = start; i < end; ++i)
            cache[i] = init[i];
    }

    /* compute terms from max(cache_len,7) up to n */
    size_t start = cache_len > 7 ? cache_len : 7;
    for (size_t k = start; k <= n; ++k) {
        seq_t pos = cache[k-1] * (seq_t)3
                  + cache[k-3] * (seq_t)4
                  + cache[k-5] * (seq_t)3;

        seq_t neg = cache[k-2] * (seq_t)4
                  + cache[k-4] * (seq_t)4
                  + cache[k-6];

        cache[k] = pos - neg;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
