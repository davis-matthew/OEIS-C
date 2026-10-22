/*
 * A315368  Coordination sequence Gal.6.46.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) + a(n-12) - a(n-13) + a(n-15) - a(n-16), with a(0)=1,
 *            a(1)=6, a(2)=11, a(3)=13, a(4)=17, a(5)=19, a(6)=23, a(7)=25,
 *            a(8)=36, a(9)=42, a(10)=48, a(11)=47, a(12)=49, a(13)=53,
 *            a(14)=55, a(15)=59, a(16)=61
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;          /* highest index already in cache */

    /* initial values for n = 0..16 */
    static const seq_t init[17] = {
        1, 6, 11, 13, 17, 19, 23, 25, 36, 42,
        48, 47, 49, 53, 55, 59, 61
    };

    /* initialise cache */
    if (cache == NULL) {
        cap = 17;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 17; ++i) {
            cache[i] = init[i];
        }
        max_computed = 16;
    }

    /* return if already computed */
    if (n <= max_computed) {
        return cache[(size_t)n];
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap = new_cap ? new_cap * 2 : 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – sequence cannot be continued */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* group positive and negative contributions to avoid unsigned underflow */
        seq_t pos = cache[(size_t)(i - 1)] + cache[(size_t)(i - 4)] +
                    cache[(size_t)(i - 7)] + cache[(size_t)(i - 9)] +
                    cache[(size_t)(i - 12)] + cache[(size_t)(i - 15)];
        seq_t neg = cache[(size_t)(i - 3)] + cache[(size_t)(i - 6)] +
                    cache[(size_t)(i - 10)] + cache[(size_t)(i - 13)] +
                    cache[(size_t)(i - 16)];
        cache[(size_t)i] = pos - neg;
    }
    max_computed = n;
    return cache[(size_t)n];
}
