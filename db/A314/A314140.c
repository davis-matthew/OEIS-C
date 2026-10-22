/*
 * A314140  Coordination sequence Gal.6.633.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, with initial
 *            values a(0) = 1, a(1) = 5, a(2) = 11, a(3) = 16, a(4) = 22, a(5)
 *            = 26, a(6) = 32, a(7) = 36, a(8) = 42, a(9) = 47, a(10) = 53,
 *            a(11) = 58, a(12) = 63
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 13 terms */
    static const seq_t init[13] = {
        1, 5, 11, 16, 22, 26, 32, 36, 42, 47, 53, 58, 63
    };

    /* Static cache of all terms computed so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of valid entries in cache */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Helper to grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort (unspecified behaviour) */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If cache is smaller than needed, compute up to n */
    if (cache_len <= (size_t)n) {
        /* Initialise the first 13 entries if we are starting out */
        if (cache_len == 0) {
            for (size_t i = 0; i < 13; ++i)
                cache[i] = init[i];
            cache_len = 13;
        }
        /* Compute remaining terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
