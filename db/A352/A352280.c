/*
 * A352280  a(0) = 1; a(n) = 3 * Sum_{k=0..floor((n-1)/2)} binomial(n-1,2*k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3 * Σ_{k=0..⌊(n-1)/2⌋} binomial(n-1, 2*k) *
 *            a(n-2*k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence uses the binomial helper from the build environment */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* number of cached elements (>=1) */

    /* initialise cache[0] on first use */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache     = (seq_t*)malloc(sizeof(seq_t));
        cache[0]   = (seq_t)1;
    }

    /* if we already know the requested value, return it immediately */
    if (n < cache_cap)
        return cache[n];

    /* enlarge the cache to hold entries up to 'n' */
    size_t old_cap = cache_cap;
    if (n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – fallback to old cache (should not happen) */
            tmp = cache;
        cache = tmp;
    }

    /* compute all new entries from old_cap to n */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        seq_t sum = 0;

        idx_t im1   = (idx_t)i - 1;          /* n-1 for this step */
        idx_t maxk  = im1 / 2;               /* floor((n-1)/2) */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* binomial(i-1, 2*k) */
            idx_t sel = 2 * k;
            seq_t bin = binomial(im1, sel);

            /* index of the term a(i-2*k-1) */
            idx_t a_index = i - 2 * k - 1;

            sum += bin * cache[a_index];
        }
        cache[i] = 3 * sum;
    }

    return cache[n];
}
